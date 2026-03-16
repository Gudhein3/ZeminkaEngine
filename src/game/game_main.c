#include "game.h"

ZEEnt_ent terrain_ent;
ZEEnt_ent player_ent;

int Game_init(int argc, char **argv) {
    terrain_ent = ZEEnt_add(terrain_intf);
    player_ent = ZEEnt_add(player_intf);
    return 0;
}

void Game_update() {
    Player_Data *data;
    ZEEnt_send(ZEENT_MSG_USR1, player_ent, &data);
    
    ZEScreen_RotateCamera(data->c_yaw, data->c_pitch, data->c_roll);
    ZEScreen_TranslateCamera(data->c_pos);
}

void Game_post_update() {
}
