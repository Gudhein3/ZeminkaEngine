#include "game.h"

void *player_alloc(ZEEnt_ent id, void *_Nullable arg) {
    Player_Data *d = calloc(sizeof(Player_Data), 1);

    Terrain_Data *data = ZEEnt_getdata(terrain_ent);
    d->c_pos.y = data->terrain[TERSIZ/2][TERSIZ/2]+64;
    d->speed = 3;
    return d;
}

void game_over(Player_Data *ent) {
    memset(ent, 0, sizeof(*ent));
    Terrain_Data *data = ZEEnt_getdata(terrain_ent);
    ent->c_pos.y = data->terrain[TERSIZ/2][TERSIZ/2]+64;
    ent->speed = 3;
    ent->vel = ZEVec3_From1(0);
}

static inline f64 clampf64(f64 v, f64 l, f64 h) {
    if (v < l) return l;
    if (v > h) return h;
    return v;
}

void player_onmsg(void *_ent, ZEEnt_ent ent_id, ZEEnt_ent caller, ZEEnt_Msg_Kind msg_kind, void *msg_data) {
    Player_Data *ent = _ent;
    switch (msg_kind) {
    case ZEENT_MSG_UPDATE: {
        if (ZEScreen_IsKeyDown(ZEKEY_w)) ent->speed += 2.*ZEdeltaTime;
        if (ZEScreen_IsKeyDown(ZEKEY_s)) ent->speed -= 2.*ZEdeltaTime;

        ent->speed = clampf64(ent->speed, 0, 100);
        ent->c_pitch -= ZEmousedY*ZEdeltaTime*6.;
        ent->c_yaw -= ZEmousedX*ZEdeltaTime*6.;

        const s64 gap = TERSIZ*TERMAP;
        const s64 x = ent->c_pos.x-fmod(ent->c_pos.x, TERSIZ*TERMAP);
        const s64 z = ent->c_pos.z-fmod(ent->c_pos.z, TERSIZ*TERMAP);
        int mapped[9][9] = {0};
        int tc = 0;
        for (ZEEnt_ent ent = 0;;++ent) {
            s32 intf = ZEEnt_get_intfid(ent);
            if (intf == -1) break;
            if (intf != INTF_TERRAIN) continue;
            Terrain_Data *data = ZEEnt_getdata(ent);
            if (data == NULL) continue;
            tc++;
            for (int i = 0; i < 9; ++i) {
                for (int j = 0; j < 9; ++j) { // 012345678
                                              // 432101234
                    if (data->x == x+(i-4)*gap && data->z == z+(j-4)*gap) {
                        mapped[j][i] = 1;
                    }
                }
            }
        }
        printf("We have %d pieces of terrain\n", tc);
        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < 9; ++j) {
                if (mapped[j][i] == 0) {
                    Terrain_InitArg terrain;
                    terrain.x = x+(i-4)*gap;
                    terrain.y = 0;
                    terrain.z = z+(j-4)*gap;
                    ZEEnt_add(INTF_TERRAIN, &terrain);
                }
            }
        }
        // TBD: full 3-component rotation.
        // if (ZEScreen_IsKeyDown(ZEKEY_Left)) ent->c_roll += ZEdeltaTime;
        // if (ZEScreen_IsKeyDown(ZEKEY_Right)) ent->c_roll -= ZEdeltaTime;
    } break;
    case ZEENT_MSG_30Hz_UPDATE: {
        ZETransformW tw = ZETransform_Cache((ZETransform) {{0}, {-ent->c_pitch, -ent->c_yaw, 0}, {1,1,1}});
        ZEVec3 fwd = ZETransformW_Apply(tw, ZEVec3_From3(0, 0, 1));

        const int air_resistance_iters = 512;
        f64 c = (1./ent->c_pos.y)*pow(DOT3(ent->vel.x, ent->vel.y, ent->vel.z, fwd.x, fwd.y, fwd.z), 2.)/air_resistance_iters;
        bool vx = ent->vel.x < 0;
        bool vy = ent->vel.y < 0;
        bool vz = ent->vel.z < 0;
        if (vx) ent->vel.x *= -1;
        if (vy) ent->vel.y *= -1;
        if (vz) ent->vel.z *= -1;
        for (int i = 0; i < air_resistance_iters; ++i) {
            ent->vel = ZEVec3_Sub(ent->vel,ZEVec3_Scale(ZEVec3_Mul(ent->vel, ent->vel),c));
        }
        if (vx) ent->vel.x *= -1;
        if (vy) ent->vel.y *= -1;
        if (vz) ent->vel.z *= -1;
        f64 g = .01;
        for (int i = 0; i < air_resistance_iters; ++i) {
            g = g-c*g*g;
        }
        ent->vel.y -= g;

        ZEVec3 vel = ZEVec3_From3(0, 0, ent->speed);
        vel = ZEVec3_Scale(vel, ZEdeltaTime);
        vel = ZETransformW_Apply(tw, vel);
        ent->vel = ZEVec3_Add(ent->vel, vel);
        ent->c_pos = ZEVec3_Add(ent->vel, ent->c_pos);
        if (ent->c_pos.y < 0) game_over(ent);
        // if (ent->c_pos.x > TERMAP*TERSIZ/4) ent->c_pos.x = -TERMAP*TERSIZ/4;
        // if (ent->c_pos.x < -TERMAP*TERSIZ/4) ent->c_pos.x = TERMAP*TERSIZ/4;
        // if (ent->c_pos.z > TERMAP*TERSIZ/4) ent->c_pos.z = -TERMAP*TERSIZ/4;
        // if (ent->c_pos.z < -TERMAP*TERSIZ/4) ent->c_pos.z = TERMAP*TERSIZ/4;
        // f64 _i = ent->c_pos.x;
        // _i += TERMAP-fmod(_i, TERMAP);
        // _i /= TERMAP;
        // _i += TERSIZ/2;
        // s64 i = _i;
        // f64 _j = ent->c_pos.z;
        // _j += TERMAP-fmod(_j, TERMAP);
        // _j /= TERMAP;
        // _j += TERSIZ/2;
        // s64 j = _j;
        //
        // f64 x = ((f64)i)-TERSIZ/2;
        // f64 y = ((f64)j)-TERSIZ/2;
        // const ZEColor c = ZEBLUE;
        // const f64 f = TERMAP;
        // Terrain_Data *tne;
        // ZEEnt_send(ZEENT_MSG_USR1, terrain_ent, &tne);
        //
        // f64 xf = fmod(ent->c_pos.x, f)/f;
        // f64 yf = fmod(ent->c_pos.z, f)/f;
        //
        // f64 a = xf * tne->terrain[j-1][i] + (1-xf) * tne->terrain[j-1][i-1];
        // f64 b = xf * tne->terrain[j][i] + (1-xf) * tne->terrain[j][i-1];
        // f64 d = yf * b + (1-yf) * a;
        //
        // if (ent->c_pos.y < d) game_over(ent);
    } break;
    }
}

void player_dealloc(void *ent, ZEEnt_ent id) {
    free(ent);
}

ZEEnt_intf player_intf = {
    player_alloc,
    player_dealloc,
    player_onmsg,
    INTF_PLAYER
};
