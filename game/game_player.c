#include "game.h"

void *player_alloc(ZEEnt_ent id) {
    Player_Data *d = calloc(sizeof(Player_Data), 1);

    Terrain_Data *data;
    ZEEnt_send(ZEENT_MSG_USR1, terrain_ent, &data);
    d->c_pos.y = data->terrain[TERSIZ/2][TERSIZ/2]+4;
    return d;
}

void game_over(Player_Data *ent) {
    memset(ent, 0, sizeof(*ent));
    Terrain_Data *data;
    ZEEnt_send(ZEENT_MSG_USR1, terrain_ent, &data);
    ent->c_pos.y = data->terrain[TERSIZ/2][TERSIZ/2]+4;
}

void player_onmsg(void *_ent, ZEEnt_ent ent_id, ZEEnt_ent caller, ZEEnt_Msg_Kind msg_kind, void *msg_data) {
    Player_Data *ent = _ent;
    switch (msg_kind) {
    case ZEENT_MSG_UPDATE: {
        if (ZEScreen_IsKeyDown(ZEKEY_up)) {
            ent->c_pitch += cos(ent->c_roll)*ZEdeltaTime;
            ent->c_yaw -= sin(ent->c_roll)*cos(-ent->c_pitch)*ZEdeltaTime;
        }
        if (ZEScreen_IsKeyDown(ZEKEY_down)) {
            ent->c_pitch -= cos(ent->c_roll)*ZEdeltaTime;
            ent->c_yaw += sin(ent->c_roll)*cos(-ent->c_pitch)*ZEdeltaTime;
        }
        if (ZEScreen_IsKeyDown(ZEKEY_left)) ent->c_roll += ZEdeltaTime;
        if (ZEScreen_IsKeyDown(ZEKEY_right)) ent->c_roll -= ZEdeltaTime;
        
        ZEVec3 vel = ZEVec3_From3(0, 0, 5);
        vel = ZEVec3_Scale(vel, ZEdeltaTime);
        ZETransformW tw = ZETransform_Cache((ZETransform) {{0}, {-ent->c_pitch, ent->c_yaw, 0}, {1,1,1}});
        vel = ZETransformW_Apply(tw, vel);
        ent->c_pos = ZEVec3_Add(vel, ent->c_pos);
    } break;
    case ZEENT_MSG_30Hz_UPDATE: {
        if (ent->c_pos.x > TERMAP*TERSIZ/4) ent->c_pos.x = -TERMAP*TERSIZ/4;
        if (ent->c_pos.x < -TERMAP*TERSIZ/4) ent->c_pos.x = TERMAP*TERSIZ/4;
        if (ent->c_pos.z > TERMAP*TERSIZ/4) ent->c_pos.z = -TERMAP*TERSIZ/4;
        if (ent->c_pos.z < -TERMAP*TERSIZ/4) ent->c_pos.z = TERMAP*TERSIZ/4;
        s64 i = ent->c_pos.x;
        i -= TERMAP-(i%(TERMAP));
        i /= TERMAP;
        i += TERSIZ/2;
        s64 j = ent->c_pos.z;
        j += TERMAP-(j%(TERMAP));
        j /= TERMAP;
        j += TERSIZ/2;
        
        f64 x = ((f64)i)-TERSIZ/2;
        f64 y = ((f64)j)-TERSIZ/2;
        const ZEColor c = ZEBLUE;
        const f64 f = TERMAP;
        Terrain_Data *tne;
        ZEEnt_send(ZEENT_MSG_USR1, terrain_ent, &tne);

        f64 xf = fmod(ent->c_pos.x, 1.);
        f64 yf = fmod(ent->c_pos.z, 1.);

        f64 a = xf * tne->terrain[j-1][i] + (1-xf) * tne->terrain[j-1][i-1];
        f64 b = xf * tne->terrain[j][i] + (1-xf) * tne->terrain[j][i-1];
        f64 d = yf * b + (1-yf) * a;

        if (ent->c_pos.y < d) game_over(ent);
    } break;
    case ZEENT_MSG_USR1: {
        *((Player_Data **)msg_data) = ent;
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
