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
            f64 x = ((f64)(i%(TERSIZ/4)))/TERSIZ+seed;
            f64 y = ((f64)(j%(TERSIZ/4)))/TERSIZ+seed;

            f64 n = 0;
            n += hash2(x+d->x, y+d->z)*4.;
            n += hash2(x-fmod(1./32.,x)+d->x, y-fmod(1./32.,y)+d->z);
            n += hash2(x-fmod(1./16.,x)+d->x, y-fmod(1./16.,y)+d->z);

            d->terrain[j][i] = n*4.;
        }
    }
    return d;
}

void terrain_onmsg(void *_ent, ZEEnt_ent ent_id, ZEEnt_ent caller, ZEEnt_Msg_Kind msg_kind, void *msg_data) {
    Terrain_Data *ent = _ent;
    switch (msg_kind) {
    case ZEENT_MSG_RENDER: {
        for (int i = 1; i < TERSIZ; ++i) {
            for (int j = 1; j < TERSIZ; ++j) {
                f64 x = ((f64)i)-TERSIZ/2;
                f64 y = ((f64)j)-TERSIZ/2;
                const f64 f = TERMAP;
                const f64 h = (ent->terrain[j-1][i]+ent->terrain[j][i-1]+ent->terrain[j-1][i-1]+ent->terrain[j][i])*.25;
                const ZEColor c = (h > 3) ? ZEWHITE : ZEBEIGE;
                ZEScreen_DrawTriangle(
                    ZEVec3_From3(x*f    +ent->x,  ent->terrain[j][i]   +ent->y, (y*f)  +ent->z),
                    ZEVec3_From3((x-1)*f+ent->x,  ent->terrain[j][i-1] +ent->y, (y*f)  +ent->z),
                    ZEVec3_From3(x*f    +ent->x,  ent->terrain[j-1][i] +ent->y, (y-1)*f+ent->z),
                    c);
                ZEScreen_DrawTriangle(
                    ZEVec3_From3(x*f    +ent->x, ent->terrain[j-1][i]  +ent->y, (y-1)*f+ent->z),
                    ZEVec3_From3((x-1)*f+ent->x, ent->terrain[j][i-1]  +ent->y, (y*f)  +ent->z),
                    ZEVec3_From3((x-1)*f+ent->x, ent->terrain[j-1][i-1]+ent->y, (y-1)*f+ent->z),
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
