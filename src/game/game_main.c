#include "game.h"

ZEEnt_ent terrain_ent;
ZEEnt_ent player_ent;

char *shader_vert = MULTILINE_STRING(
\043version 330\n
attribute vec4 vPosition;
varying vec2 fPosition;
void main() {
    fPosition = (vPosition.xy+1.)*.5;
    gl_Position = vPosition;
}
);

char *shader_frag = MULTILINE_STRING(
\043version 330\n
uniform float uSysTime;
varying vec2 fPosition;
uniform sampler2D screen;
uniform sampler2D screen_depth;
void main() {
    vec4 a = texture2D(screen, fPosition);
    gl_FragColor = a;
}
);

static ZEShaderProg shader;

int Game_init(int argc, char **argv) {
    ZEScreen_CompileShader(&shader, shader_vert, shader_frag);
    ZEEnt_register_intf(terrain_intf);
    ZEEnt_register_intf(player_intf);
    terrain_ent = ZEEnt_add(INTF_TERRAIN, NULL);
    player_ent = ZEEnt_add(INTF_PLAYER, NULL);
    return 0;
}

void Game_update() {
    Player_Data *data = ZEEnt_getdata(player_ent);

    ZEScreen_RotateCamera(data->c_yaw, data->c_pitch, data->c_roll);
    ZEScreen_TranslateCamera(data->c_pos);
}

void Game_post_update() {
    ZEScreen_DrawShaderTriangle(&shader, ZEVec2_From2(-1, -1), ZEVec2_From2(1, -1), ZEVec2_From2(-1, 1));
    ZEScreen_DrawShaderTriangle(&shader, ZEVec2_From2(1, 1), ZEVec2_From2(-1, 1), ZEVec2_From2(1, -1));
}
