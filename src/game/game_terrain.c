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

void *terrain_alloc(ZEEnt_ent id) {
    Terrain_Data *d = malloc(sizeof(Terrain_Data));
    int seed = time(NULL);
    for (int i = 0; i < TERSIZ; ++i) {
        for (int j = 0; j < TERSIZ; ++j) {
            f64 x = ((f64)(i%(TERSIZ/4)))/TERSIZ+seed;
            f64 y = ((f64)(j%(TERSIZ/4)))/TERSIZ+seed;
            f64 n = 0;
            n += hash2(x, y)/2.;
            n += hash2(x-fmod(1./32.,x), y-fmod(1./32.,y))/4.;
            n += hash2(x-fmod(1./16.,x), y-fmod(1./16.,y))/8.;
            n += hash2(x-fmod(1./8.,x),  y-fmod(1./8.,y))/16.;
            n += hash2(x-fmod(1./4.,x),  y-fmod(1./4.,y))/32.;
            n += hash2(x-fmod(1./2.,x),  y-fmod(1./2.,y))/64.;
            
            n += pow(hash2(x+5623, y+6931), 2);
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
                    ZEVec3_From3(x*f, ent->terrain[j][i],   (y*f)),
                    ZEVec3_From3((x-1)*f, ent->terrain[j][i-1], (y*f)),
                    ZEVec3_From3(x*f, ent->terrain[j-1][i], (y-1)*f),
                    c);
                ZEScreen_DrawTriangle(
                    ZEVec3_From3((x-1)*f, ent->terrain[j-1][i-1], (y-1)*f),
                    ZEVec3_From3((x-1)*f, ent->terrain[j][i-1],   (y*f)),
                    ZEVec3_From3(x*f, ent->terrain[j-1][i],   (y-1)*f),
                    c);
            }
        }
    } break;
    case ZEENT_MSG_USR1: {
        *((Terrain_Data **)msg_data) = ent;
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
