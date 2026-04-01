#pragma once
#include <zeminka/main.h>

typedef enum {
    ZEKEY_ESC          = 1,
    ZEKEY_Backtick     = 2,
    ZEKEY_Grave        = ZEKEY_Backtick,
    ZEKEY_0            = 3,
    ZEKEY_1            = 4,
    ZEKEY_2            = 5,
    ZEKEY_3            = 6,
    ZEKEY_4            = 7,
    ZEKEY_5            = 8,
    ZEKEY_6            = 9,
    ZEKEY_7            = 10,
    ZEKEY_8            = 11,
    ZEKEY_9            = 12,
    ZEKEY_Minus        = 13,
    ZEKEY_Equal        = 14,
    ZEKEY_BackSpace    = 15,
    ZEKEY_Tab          = 16,
    ZEKEY_Space        = 17,
    ZEKEY_a            = 18,
    ZEKEY_b            = 19,
    ZEKEY_c            = 20,
    ZEKEY_d            = 21,
    ZEKEY_e            = 22,
    ZEKEY_f            = 23,
    ZEKEY_g            = 24,
    ZEKEY_h            = 25,
    ZEKEY_i            = 26,
    ZEKEY_j            = 27,
    ZEKEY_k            = 28,
    ZEKEY_l            = 29,
    ZEKEY_m            = 30,
    ZEKEY_n            = 31,
    ZEKEY_o            = 32,
    ZEKEY_p            = 33,
    ZEKEY_q            = 34,
    ZEKEY_r            = 35,
    ZEKEY_s            = 36,
    ZEKEY_t            = 37,
    ZEKEY_u            = 38,
    ZEKEY_v            = 39,
    ZEKEY_w            = 40,
    ZEKEY_x            = 41,
    ZEKEY_y            = 42,
    ZEKEY_z            = 43,
    ZEKEY_Period       = 44,
    ZEKEY_Comma        = 45,
    ZEKEY_Slash        = 46,
    ZEKEY_Bracket      = 47,
    ZEKEY_Closebracket = 48,
    ZEKEY_Semicolon    = 49,
    ZEKEY_Apostrophe   = 50,
    ZEKEY_Backslash    = 51,
    ZEKEY_Return       = 52,
    ZEKEY_Enter        = 53,
    ZEKEY_Delete       = 54,
    ZEKEY_F1           = 55,
    ZEKEY_F2           = 56,
    ZEKEY_F3           = 57,
    ZEKEY_F4           = 58,
    ZEKEY_F5           = 59,
    ZEKEY_F6           = 60,
    ZEKEY_F7           = 61,
    ZEKEY_F8           = 62,
    ZEKEY_F9           = 63,
    ZEKEY_F10          = 64,
    ZEKEY_F11          = 65,
    ZEKEY_F12          = 66,
    ZEKEY_F13          = 67,
    ZEKEY_F14          = 68,
    ZEKEY_F15          = 69,
    ZEKEY_F16          = 70,
    ZEKEY_F17          = 71,
    ZEKEY_F18          = 72,
    ZEKEY_F19          = 73,
    ZEKEY_F20          = 74,
    ZEKEY_F21          = 75,
    ZEKEY_F22          = 76,
    ZEKEY_F23          = 77,
    ZEKEY_F24          = 78,
    ZEKEY_F25          = 79,
    ZEKEY_capsLock     = 80,
    ZEKEY_ShiftL       = 81,
    ZEKEY_ControlL     = 82,
    ZEKEY_AltL         = 83,
    ZEKEY_SuperL       = 84,
    ZEKEY_ShiftR       = 85,
    ZEKEY_ControlR     = 86,
    ZEKEY_AltR         = 87,
    ZEKEY_SuperR       = 88,
    ZEKEY_Up           = 89,
    ZEKEY_Down         = 90,
    ZEKEY_Left         = 91,
    ZEKEY_Right        = 92,
    ZEKEY_Insert       = 93,
    ZEKEY_Menu         = 94,
    ZEKEY_End          = 95,
    ZEKEY_Home         = 96,
    ZEKEY_Pageup       = 97,
    ZEKEY_Pagedown     = 98,
    ZEKEY_NumLock      = 99,
    ZEKEY_KPslash      = 100,
    ZEKEY_KPmultiply   = 101,
    ZEKEY_KPplus       = 102,
    ZEKEY_KPminus      = 103,
    ZEKEY_KPequal      = 104,
    ZEKEY_KP1          = 105,
    ZEKEY_KP2          = 106,
    ZEKEY_KP3          = 107,
    ZEKEY_KP4          = 108,
    ZEKEY_KP5          = 109,
    ZEKEY_KP6          = 110,
    ZEKEY_KP7          = 111,
    ZEKEY_KP8          = 112,
    ZEKEY_KP9          = 113,
    ZEKEY_KP0          = 114,
    ZEKEY_KPperiod     = 115,
    ZEKEY_KPreturn     = 116,
    ZEKEY_ScrollLock   = 117,
    ZEKEY_PrintScreen  = 118,
    ZEKEY_Pause        = 119,
    ZEKEY_World1       = 120,
    ZEKEY_World2       = 121,

    ZEKEY_First        = ZEKEY_ESC,
    ZEKEY_Last         = ZEKEY_World2
} ZEKey;

typedef struct {
    u64 vert;
    u64 frag;
    u64 prog;
} ZEShaderProg;

enum {
    ZEScreenFlag_Nothing    = 0,
    ZEScreenFlag_Resizeable = 1<<0,
    ZEScreenFlag_Borderless = 1<<1,
    ZEScreenFlag_Fullscreen = 1<<2,
};

#define DEG2RAD (PI/180)
#define RAD2DEG (180/PI)

#ifndef _ZE_NO_SYMBOLS
void ZEScreen_init(u32 width, u32 height, f64 fov, const char *title, u32 flags);

u64 ZEScreen_CompileShader(ZEShaderProg *prog, const char *vert, const char *frag);

void ZEScreen_DrawShaderTriangle(ZEShaderProg *prog, ZEVec2 a, ZEVec2 b, ZEVec2 c);
void ZEScreen_Shader_SetUniformf(ZEShaderProg *prog, const char *name, f64 param);
void ZEScreen_Shader_SetUniformi(ZEShaderProg *prog, const char *name, int param);
void ZEScreen_Shader_SetUniformf2(ZEShaderProg *prog, const char *name, ZEVec2 param);
void ZEScreen_Shader_SetUniformf3(ZEShaderProg *prog, const char *name, ZEVec3 param);
void ZEScreen_Shader_SetUniformf4(ZEShaderProg *prog, const char *name, ZEVec4 param);
bool ZEScreen_IsClosed();
bool ZEScreen_IsNtClosed(); // Finally isn't closed function!

void ZEScreen_BeginFrame(f64 *_Nullable mdx, f64 *_Nullable mdy); // mouse delta, not an absolute position.
void ZEScreen_ResetCamera();
void ZEScreen_TranslateCamera(ZEVec3 origin);
void ZEScreen_RotateCamera(f64 yaw, f64 pitch, f64 roll); // YZ,XZ,XY
void ZEScreen_EndFrame();

void ZEScreen_DrawCircle(ZEVec3 o, f64 r, ZEColor col);
void ZEScreen_DrawTriangleRaw(ZEVec3 a, ZEVec3 b, ZEVec3 c, ZEColor col);
void ZEScreen_DrawTriangle(ZEVec3 a, ZEVec3 b, ZEVec3 c, ZEColor col);
void ZEScreen_DrawTriangle_Ex(ZEVec3 a, ZEVec3 b, ZEVec3 c, ZEColor a_c, ZEColor b_c, ZEColor c_c);

bool ZEScreen_IsKeyPressed(ZEKey key);
bool ZEScreen_IsKeyDown(ZEKey key);
bool ZEScreen_IsKeyReleased(ZEKey key);

bool ZEScreen_GetDepthTest();
void ZEScreen_SetDepthTest(bool test);

void ZEScreen_RenderModel(ZEModel model, ZETransformW transform);

void *ZEScreen_GetSystemHandler();
#endif  // _ZE_NO_SYMBOLS

extern f64 ZEmousedX, ZEmousedY;

#define ZERED ((ZEColor){1.,0,0,1.})
#define ZEMAGENTA ((ZEColor){1.,0,1.,1.})
#define ZEMYCOLOR ((ZEColor){.894,.878,0,1.})
#define ZEGREEN ((ZEColor){0,1.,0,1.})
#define ZECYAN ((ZEColor){0,1.,1.,1.})
#define ZEBLUE ((ZEColor){0,0,1.,1.})
#define ZEBLACK ((ZEColor){0,0,0,1.})
#define ZEWHITE ((ZEColor){1.,1.,1.,1.})
#define ZEGRAY ((ZEColor){.5,.5,.5,1.})
#define ZEORANGE ((ZEColor){1.,.5,0,1.})
#define ZEBEIGE ((ZEColor){.960, .960, .862})

// Small hack for the MSVC compiler.
#define ZERED_ {1.,0,0,1.}
#define ZEMAGENTA_ {1.,0,1.,1.}
#define ZEMYCOLOR_ {.894,.878,0,1.}
#define ZEGREEN_ {0,1.,0,1.}
#define ZECYAN_ {0,1.,1.,1.}
#define ZEBLUE_ {0,0,1.,1.}
#define ZEBLACK_ {0,0,0,1.}
#define ZEWHITE_ {1.,1.,1.,1.}
#define ZEGRAY_ {.5,.5,.5,1.}
#define ZEORANGE_ {1.,.5,0,1.}
#define ZEBEIGE_ {.960, .960, .862}
