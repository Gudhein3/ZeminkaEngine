#pragma once

#include <stdio.h>
#include <zeminka/engine.h>

#define INTF_TERRAIN 0
#define INTF_PLAYER 1
#define TERSIZ 80
#define TERMAP 6

typedef struct {
    f64 terrain[TERSIZ][TERSIZ];
} Terrain_Data;

extern ZEEnt_intf terrain_intf;

typedef struct {
    f64 c_yaw;
    f64 c_pitch;
    f64 c_roll;
    ZEVec3 c_pos;
} Player_Data;

extern ZEEnt_intf player_intf;
extern ZEEnt_ent terrain_ent;
extern ZEEnt_ent player_ent;
