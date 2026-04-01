#pragma once

#include <stdio.h>
#include <zeminka/engine.h>

#define INTF_TERRAIN 0
#define INTF_PLAYER 1
#define TERSIZ 60
#define TERMAP 32

typedef struct {
    f64 terrain[TERSIZ][TERSIZ];
    s64 x, y, z;
} Terrain_Data;

typedef struct {
    f64 x, y, z;
} Terrain_InitArg;

extern ZEEnt_intf terrain_intf;

typedef struct {
    f64 c_yaw;
    f64 c_pitch;
    f64 c_roll;
    ZEVec3 c_pos;
    f64 speed;
    ZEVec3 vel;
} Player_Data;

extern ZEEnt_intf player_intf;
extern ZEEnt_ent terrain_ent;
extern ZEEnt_ent player_ent;
