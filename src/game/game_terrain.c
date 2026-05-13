#include "game.h"

static f64 hash2(f64 x, f64 y) // Stolen from https://github.com/mattdesl/glsl-random/blob/master/index.glsl
{
    f64 a  = 12.9898;
    f64 b  = 78.233;
    f64 c  = 43758.5453;
    f64 dt = (x*a)+(y*b);
    f64 sn = fmod(dt,3.14);
    return fmodf(sin(sn) * c, 1.);
}

void *terrain_alloc(ZEEnt_ent id, void *_Nullable arg) {
    Terrain_Data *d = calloc(1, sizeof(Terrain_Data));
    if (arg) {
        Terrain_InitArg *_arg = arg;
        d->x = _arg->x;
        d->y = _arg->y;
        d->z = _arg->z;
    }
    int seed = time(NULL);
    for (int i = 0; i < TERSIZ; ++i) {
        for (int j = 0; j < TERSIZ; ++j) {
            f64 x = ((f64)i)/TERSIZ+seed+d->x;
            f64 y = ((f64)j)/TERSIZ+seed+d->z;

            f64 n = 0;
            n += powf(hash2(x-fmod(1./32.,x), y-fmod(1./32.,y)), 64.)*64.*hash2(x, y);
            n += powf(hash2(x, y), 32.)*32.;
            n += powf(hash2(x, y), 16.)*16.;
            n += powf(hash2(x, y), 8.)*8.;
            n += hash2(x-fmod(1./32.,x), y-fmod(1./32.,y));
            n += hash2(x-fmod(1./16.,x), y-fmod(1./16.,y));

            d->terrain[j][i] = n*4.;
        }
    }
    return d;
}

void terrain_onmsg(void *_ent, ZEEnt_ent ent_id, ZEEnt_ent caller, ZEEnt_Msg_Kind msg_kind, void *msg_data) {
    Terrain_Data *ent = _ent;
    switch (msg_kind) {
    case ZEENT_MSG_RENDER: { // TBD: Two terrain pieces don't connect perfectly
        for (int _i = 0; _i <= TERSIZ; ++_i) {
            for (int _j = 0; _j <= TERSIZ; ++_j) {
                int i = _i%TERSIZ;
                int j = _j%TERSIZ;
                f64 x = ((f64)i)-TERSIZ/2;
                f64 y = ((f64)j)-TERSIZ/2;
                const f64 f = TERMAP;
                const f64 h = (ent->terrain[_j-1][i]+ent->terrain[j][_i-1]+ent->terrain[_j-1][_i-1]+ent->terrain[j][i])*.25;
                const ZEColor c = (h > 30) ? ZEWHITE : ZEBEIGE;
                ZEScreen_DrawTriangle(
                    ZEVec3_From3(x*f    +ent->x,  ent->terrain[j][i]   +ent->y, (y*f)  +ent->z),
                    ZEVec3_From3((x-1)*f+ent->x,  _i == 0 ? 0 : ent->terrain[j][_i-1] +ent->y, (y*f)  +ent->z),
                    ZEVec3_From3(x*f    +ent->x,  _j == 0 ? 0 : ent->terrain[_j-1][i] +ent->y, (y-1)*f+ent->z),
                    c);
                ZEScreen_DrawTriangle(
                    ZEVec3_From3(x*f    +ent->x, _j == 0 ? 0 : ent->terrain[_j-1][i]  +ent->y, (y-1)*f+ent->z),
                    ZEVec3_From3((x-1)*f+ent->x, _i == 0 ? 0 :ent->terrain[j][_i-1]  +ent->y, (y*f)  +ent->z),
                    ZEVec3_From3((x-1)*f+ent->x, _i == 0 || _j == 0 ? 0 : ent->terrain[_j-1][_i-1]+ent->y, (y-1)*f+ent->z),
                    c);
            }
        }
    } break;
    }
}

void terrain_dealloc(void *ent, ZEEnt_ent id) {
    free(ent);
}

ZEEnt_intf terrain_intf = {
    terrain_alloc,
    terrain_dealloc,
    terrain_onmsg,
    INTF_TERRAIN
};
