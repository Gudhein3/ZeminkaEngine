#include <zeminka/engine.h>

#include <stdarg.h>

#include <stdlib.h>
#include <stdio.h>

#ifdef _WIN32
#include <windows.h>
#endif

#include <time.h>

static double getSystemTime() { // TODO: Make it public.
#if defined(_WIN32)
    FILETIME system_time;
    ULARGE_INTEGER large;

    GetSystemTimePreciseAsFileTime(&system_time);
    large.u.LowPart = system_time.dwLowDateTime;
    large.u.HighPart = system_time.dwHighDateTime;
    const u64 scale_factor = 1000000;
    u64 q = large.QuadPart/10;
    return (f64)q/(f64)scale_factor;
#elif defined(__APPLE__)
    zetodo("Apple MacOSX");
#else
    // Stolen from RGFW and rewrotten with floats.
    struct timespec ts;
    const u64 scale_factor = 1000000000;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (f64)ts.tv_sec + (f64)ts.tv_nsec / (f64)scale_factor;
#endif
}

static u64 getUnxTime() { // Un*x is a forbidden word.
    return time(NULL);
}

static double getDeltaTime() {
    static double latest = 0;
    double platest = latest;
    latest = getSystemTime();
    if (latest == 0)
        return 0;
    return latest-platest;
}

const ZEVertex player_model_v[] = {
    {1,   1,   1},  // 0
    {1,   1,   -1}, // 1
    {1,   -1,  1},  // 2
    {1,   -1,  -1}, // 3
    {-1,  1,   1},  // 4
    {-1,  1,   -1}, // 5
    {-1,  -1,  1},  // 6
    {-1,  -1,  -1}, // 7
};

const ZEColor player_model_c[] = {
    ZERED_,
    ZEMAGENTA_,
    ZEGREEN_,
    ZECYAN_,
    ZEBLUE_,
    ZEBLACK_,
    ZEWHITE_,
    ZEORANGE_
};

const ZEFace player_model_f[] = {
    {0, 1, 5},
    {5, 4, 0},

    {5, 1, 3},
    {3, 5, 7},

    {0, 2, 3},
    {3, 1, 0},

    {5, 4, 6},
    {6, 7, 5},

    {2, 0, 4},
    {4, 6, 2},

    {2, 3, 7},
    {7, 6, 2},
};

const ZEModel player_model = {
    player_model_v,
    player_model_c,
    player_model_f,
    12
};

f64 ZEmousedX = 0, ZEmousedY = 0;

static FILE *_logf;
f64 ZEdeltaTime, ZEsystemTime, ZEdeltaTime30Hz;

int Game_init(int argc, char **argv);
void Game_update();
void Game_post_update();

int main(int argc, char **argv) {
    ZEScreen_init(640, 480, 120., "Flight simulator", ZEScreenFlag_Fullscreen | ZEScreenFlag_Borderless);

    _logf = fopen("zeminkaengine.log", "wb");
    if (!_logf)
        ZELog(ZELOG_FATAL, "Failed to open log file `zeminkaengine.log'.");

    ZELog(ZELOG_INFO, "Successfully opened log file `zeminkaengine.log'.");

    ZESndInit();

    ZELog(ZELOG_INFO, "Successfully initialized the sound engine.");

    ZEScreen_BeginFrame(&ZEmousedX, &ZEmousedY);
    
    getDeltaTime(); // Refresh delta time.

    {
        int code = Game_init(argc, argv);
        if (code != 0) return code;
    }
    
    while (ZEScreen_IsNtClosed()) {
        ZEdeltaTime = getDeltaTime();
        ZEsystemTime = getSystemTime();
        ZEScreen_BeginFrame(&ZEmousedX, &ZEmousedY);
        Game_update();
        ZEEnt_update();
        Game_post_update();
        ZEScreen_EndFrame();
    }
}

ZEVec2 ZEVec2_Rotate(ZEVec2 v, ZERotation r) {return ZEVec2_From2(v.x*r.cos-v.y*r.sin,v.x*r.sin+v.y*r.cos);}
ZEVec3 ZEVec3_RotateXY(ZEVec3 v, ZERotation r) {
    f64 dx = v.x*r.cos-v.y*r.sin, dy = v.x*r.sin+v.y*r.cos;
    v.x = dx;
    v.y = dy;
    return v;
}
ZEVec3 ZEVec3_RotateYZ(ZEVec3 v, ZERotation r) {
    f64 dy = v.y*r.cos-v.z*r.sin, dz = v.y*r.sin+v.z*r.cos;
    v.y = dy;
    v.z = dz;
    return v;
}
ZEVec3 ZEVec3_RotateXZ(ZEVec3 v, ZERotation r) {
    f64 dx = v.x*r.cos-v.z*r.sin, dz = v.x*r.sin+v.z*r.cos;
    v.x = dx;
    v.z = dz;
    return v;
}

ZEVec2 ZEVec2_Add(ZEVec2 a, ZEVec2 b) {return ZEVec2_From2(a.x+b.x,a.y+b.y);}
ZEVec2 ZEVec2_Sub(ZEVec2 a, ZEVec2 b) {return ZEVec2_From2(a.x-b.x,a.y-b.y);}
ZEVec2 ZEVec2_Mul(ZEVec2 a, ZEVec2 b) {return ZEVec2_From2(a.x*b.x,a.y*b.y);}
ZEVec2 ZEVec2_Div(ZEVec2 a, ZEVec2 b) {return ZEVec2_From2(a.x/b.x,a.y/b.y);}
ZEVec2 ZEVec2_Scale(ZEVec2 v, f64 s) {return ZEVec2_From2(v.x*s,v.y*s);}

ZEVec3 ZEVec3_Add(ZEVec3 a, ZEVec3 b) {return ZEVec3_From3(a.x+b.x,a.y+b.y,a.z+b.z);}
ZEVec3 ZEVec3_Sub(ZEVec3 a, ZEVec3 b) {return ZEVec3_From3(a.x-b.x,a.y-b.y,a.z-b.z);}
ZEVec3 ZEVec3_Mul(ZEVec3 a, ZEVec3 b) {return ZEVec3_From3(a.x*b.x,a.y*b.y,a.z*b.z);}
ZEVec3 ZEVec3_Div(ZEVec3 a, ZEVec3 b) {return ZEVec3_From3(a.x/b.x,a.y/b.y,a.z/b.z);}
ZEVec3 ZEVec3_Scale(ZEVec3 v, f64 s) {return ZEVec3_From3(v.x*s,v.y*s,v.z*s);}

f64    ZEVec3_MagSq(ZEVec3 v) {return v.x*v.x+v.y*v.y+v.z*v.z;}
f64    ZEVec3_Mag(ZEVec3 v) {return sqrt(v.x*v.x+v.y*v.y+v.z*v.z);}
ZEVec3 ZEVec3_Norm(ZEVec3 v) {
    if (v.x == 0 && v.y == 0 && v.z == 0) return v;
    f64 m = ZEVec3_Mag(v);
    return ZEVec3_From3(v.x/m,v.y/m,v.z/m);
}
f64    ZEVec3_Dist(ZEVec3 a, ZEVec3 b) {
    return ZEVec3_Mag(ZEVec3_Sub(a, b));
}

ZEVec4 ZEVec4_Add(ZEVec4 a, ZEVec4 b) {return ZEVec4_From4(a.x+b.x,a.y+b.y,a.z+b.z,a.w+b.w);}
ZEVec4 ZEVec4_Sub(ZEVec4 a, ZEVec4 b) {return ZEVec4_From4(a.x-b.x,a.y-b.y,a.z-b.z,a.w-b.w);}
ZEVec4 ZEVec4_Mul(ZEVec4 a, ZEVec4 b) {return ZEVec4_From4(a.x*b.x,a.y*b.y,a.z*b.z,a.w*b.w);}
ZEVec4 ZEVec4_Div(ZEVec4 a, ZEVec4 b) {return ZEVec4_From4(a.x/b.x,a.y/b.y,a.z/b.z,a.w/b.w);}
ZEVec4 ZEVec4_Scale(ZEVec4 v, f64 s) {return ZEVec4_From4(v.x*s,v.y*s,v.z*s,v.w*s);}

ZETransformW ZETransform_Cache(ZETransform t) {
    return (ZETransformW) {
        t.position,
        t.scale,
        ZERotation_From_Rad(t.rotation.z),
        ZERotation_From_Rad(t.rotation.y),
        ZERotation_From_Rad(t.rotation.x),
    };
}

ZEVec3 ZETransformW_Apply(ZETransformW t, ZEVec3 v) {
    // RST
    v = ZEVec3_RotateYZ(v, t.cryz);
    v = ZEVec3_RotateXZ(v, t.crxz);
    v = ZEVec3_RotateXY(v, t.crxy);

    v = ZEVec3_Mul(v, t.scale);

    v = ZEVec3_Add(v, t.position);

    return v;
}

void ZELog(ZELogLevel ll, const char *fmt, ...) {
    va_list list0, list1;
    va_start(list0, fmt);
    va_copy(list1, list0);
    size_t cnt = vsnprintf(NULL, 0, fmt, list0);
    char *buf = malloc(cnt+1);
    buf[cnt] = 0;
    vsnprintf(buf, cnt+1, fmt, list1);
    static const char *llses[] = {
        "INFO",
        "TODO",
        "WARNING",
        "ERROR",
        "FATAL"
    };
    const char *lls = llses[ll];
    u64 st = getUnxTime();
    printf("[%s] (%llu) %s\n", lls, st, buf);
    if (_logf) fprintf(_logf, "[%s] (%llu) %s\n", lls, st, buf);

    free(buf);
    va_end(list0);
    va_end(list1);

    if (ll == ZELOG_FATAL) {
        // The OS will close file when the program close so, no `fclose(_logf);'.
        exit(1);
    }
}
