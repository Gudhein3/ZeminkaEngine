#include <zeminka/engine.h>
#include <stdio.h>

#include <GLFW/glfw3.h>
#include <GL/gl.h>

static GLFWwindow *rwin;

static f64 rwidth, rheight;
static f64 rfov;

static int key_mapping[] = {
    [ZEKEY_ESC] = GLFW_KEY_ESCAPE,
    [ZEKEY_Backtick] = GLFW_KEY_GRAVE_ACCENT,
    [ZEKEY_Grave] = GLFW_KEY_GRAVE_ACCENT,
    [ZEKEY_0] = GLFW_KEY_0,
    [ZEKEY_1] = GLFW_KEY_1,
    [ZEKEY_2] = GLFW_KEY_2,
    [ZEKEY_3] = GLFW_KEY_3,
    [ZEKEY_4] = GLFW_KEY_4,
    [ZEKEY_5] = GLFW_KEY_5,
    [ZEKEY_6] = GLFW_KEY_6,
    [ZEKEY_7] = GLFW_KEY_7,
    [ZEKEY_8] = GLFW_KEY_8,
    [ZEKEY_9] = GLFW_KEY_9,
    [ZEKEY_Minus] = GLFW_KEY_MINUS,
    [ZEKEY_Equal] = GLFW_KEY_EQUAL,
    [ZEKEY_BackSpace] = GLFW_KEY_BACKSPACE,
    [ZEKEY_Tab] = GLFW_KEY_TAB,
    [ZEKEY_Space] = GLFW_KEY_SPACE,
    [ZEKEY_a] = GLFW_KEY_A,
    [ZEKEY_b] = GLFW_KEY_B,
    [ZEKEY_c] = GLFW_KEY_C,
    [ZEKEY_d] = GLFW_KEY_D,
    [ZEKEY_e] = GLFW_KEY_E,
    [ZEKEY_f] = GLFW_KEY_F,
    [ZEKEY_g] = GLFW_KEY_G,
    [ZEKEY_h] = GLFW_KEY_H,
    [ZEKEY_i] = GLFW_KEY_I,
    [ZEKEY_j] = GLFW_KEY_J,
    [ZEKEY_k] = GLFW_KEY_K,
    [ZEKEY_l] = GLFW_KEY_L,
    [ZEKEY_m] = GLFW_KEY_M,
    [ZEKEY_n] = GLFW_KEY_N,
    [ZEKEY_o] = GLFW_KEY_O,
    [ZEKEY_p] = GLFW_KEY_P,
    [ZEKEY_q] = GLFW_KEY_Q,
    [ZEKEY_r] = GLFW_KEY_R,
    [ZEKEY_s] = GLFW_KEY_S,
    [ZEKEY_t] = GLFW_KEY_T,
    [ZEKEY_u] = GLFW_KEY_U,
    [ZEKEY_v] = GLFW_KEY_V,
    [ZEKEY_w] = GLFW_KEY_W,
    [ZEKEY_x] = GLFW_KEY_X,
    [ZEKEY_y] = GLFW_KEY_Y,
    [ZEKEY_z] = GLFW_KEY_Z,
    [ZEKEY_Period] = GLFW_KEY_PERIOD,
    [ZEKEY_Comma] = GLFW_KEY_COMMA,
    [ZEKEY_Slash] = GLFW_KEY_SLASH,
    [ZEKEY_Bracket] = GLFW_KEY_LEFT_BRACKET,
    [ZEKEY_Closebracket] = GLFW_KEY_RIGHT_BRACKET,
    [ZEKEY_Semicolon] = GLFW_KEY_SEMICOLON,
    [ZEKEY_Apostrophe] = GLFW_KEY_APOSTROPHE,
    [ZEKEY_Backslash] = GLFW_KEY_BACKSLASH,
    [ZEKEY_Return] = GLFW_KEY_ENTER,
    [ZEKEY_Enter] = GLFW_KEY_ENTER,
    [ZEKEY_Delete] = GLFW_KEY_DELETE,
    [ZEKEY_F1] = GLFW_KEY_F1,
    [ZEKEY_F2] = GLFW_KEY_F2,
    [ZEKEY_F3] = GLFW_KEY_F3,
    [ZEKEY_F4] = GLFW_KEY_F4,
    [ZEKEY_F5] = GLFW_KEY_F5,
    [ZEKEY_F6] = GLFW_KEY_F6,
    [ZEKEY_F7] = GLFW_KEY_F7,
    [ZEKEY_F8] = GLFW_KEY_F8,
    [ZEKEY_F9] = GLFW_KEY_F9,
    [ZEKEY_F10] = GLFW_KEY_F10,
    [ZEKEY_F11] = GLFW_KEY_F11,
    [ZEKEY_F12] = GLFW_KEY_F12,
    [ZEKEY_F13] = GLFW_KEY_F13,
    [ZEKEY_F14] = GLFW_KEY_F14,
    [ZEKEY_F15] = GLFW_KEY_F15,
    [ZEKEY_F16] = GLFW_KEY_F16,
    [ZEKEY_F17] = GLFW_KEY_F17,
    [ZEKEY_F18] = GLFW_KEY_F18,
    [ZEKEY_F19] = GLFW_KEY_F19,
    [ZEKEY_F20] = GLFW_KEY_F20,
    [ZEKEY_F21] = GLFW_KEY_F21,
    [ZEKEY_F22] = GLFW_KEY_F22,
    [ZEKEY_F23] = GLFW_KEY_F23,
    [ZEKEY_F24] = GLFW_KEY_F24,
    [ZEKEY_F25] = GLFW_KEY_F25,
    [ZEKEY_capsLock] = GLFW_KEY_CAPS_LOCK,
    [ZEKEY_ShiftL] = GLFW_KEY_LEFT_SHIFT,
    [ZEKEY_ControlL] = GLFW_KEY_LEFT_CONTROL,
    [ZEKEY_AltL] = GLFW_KEY_LEFT_ALT,
    [ZEKEY_SuperL] = GLFW_KEY_LEFT_SUPER,
    [ZEKEY_ShiftR] = GLFW_KEY_RIGHT_SHIFT,
    [ZEKEY_ControlR] = GLFW_KEY_RIGHT_CONTROL,
    [ZEKEY_AltR] = GLFW_KEY_RIGHT_ALT,
    [ZEKEY_SuperR] = GLFW_KEY_RIGHT_SUPER,
    [ZEKEY_Up] = GLFW_KEY_UP,
    [ZEKEY_Down] = GLFW_KEY_DOWN,
    [ZEKEY_Left] = GLFW_KEY_LEFT,
    [ZEKEY_Right] = GLFW_KEY_RIGHT,
    [ZEKEY_Insert] = GLFW_KEY_INSERT,
    [ZEKEY_Menu] = GLFW_KEY_MENU,
    [ZEKEY_End] = GLFW_KEY_END,
    [ZEKEY_Home] = GLFW_KEY_HOME,
    [ZEKEY_Pageup] = GLFW_KEY_PAGE_UP,
    [ZEKEY_Pagedown] = GLFW_KEY_PAGE_DOWN,
    [ZEKEY_NumLock] = GLFW_KEY_NUM_LOCK,
    [ZEKEY_KPslash] = GLFW_KEY_KP_DIVIDE,
    [ZEKEY_KPmultiply] = GLFW_KEY_KP_MULTIPLY,
    [ZEKEY_KPplus] = GLFW_KEY_KP_ADD,
    [ZEKEY_KPminus] = GLFW_KEY_KP_SUBTRACT,
    [ZEKEY_KPequal] = GLFW_KEY_KP_EQUAL,
    [ZEKEY_KP1] = GLFW_KEY_KP_1,
    [ZEKEY_KP2] = GLFW_KEY_KP_2,
    [ZEKEY_KP3] = GLFW_KEY_KP_3,
    [ZEKEY_KP4] = GLFW_KEY_KP_4,
    [ZEKEY_KP5] = GLFW_KEY_KP_5,
    [ZEKEY_KP6] = GLFW_KEY_KP_6,
    [ZEKEY_KP7] = GLFW_KEY_KP_7,
    [ZEKEY_KP8] = GLFW_KEY_KP_8,
    [ZEKEY_KP9] = GLFW_KEY_KP_9,
    [ZEKEY_KP0] = GLFW_KEY_KP_0,
    [ZEKEY_KPperiod] = GLFW_KEY_KP_DECIMAL,
    [ZEKEY_KPreturn] = GLFW_KEY_KP_ENTER,
    [ZEKEY_ScrollLock] = GLFW_KEY_SCROLL_LOCK,
    [ZEKEY_PrintScreen] = GLFW_KEY_PRINT_SCREEN,
    [ZEKEY_Pause] = GLFW_KEY_PAUSE,
    [ZEKEY_World1] = GLFW_KEY_WORLD_1,
    [ZEKEY_World2] = GLFW_KEY_WORLD_2
};

#define KEYST_PRESSED 1
#define KEYST_DOWN 2
#define KEYST_RELEASED 4

static int key_status[1024] = {0};

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_ESCAPE) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
        key_status[key] |= KEYST_PRESSED;
        key_status[key] |= KEYST_DOWN;
    }
    if (action == GLFW_RELEASE)
        key_status[key] |= KEYST_RELEASED;
}

static u8 icon[16 * 16 * 3 + 1] = // Silly Placeholder
    "\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000"
    "\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000J\000\377J"
    "\000\377\060\377\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000J\000\377J\000\377J\000"
    "\377J\000\377J\000\377J\000\377J\000\377\377\000\000J\000\377J\000\377J\000\377\060\377\000\377"
    "\000\000\377\000\000\377\000\000\377\000\000J\000\377\060\377\000\060\377\000\060\377\000\060\377\000\060"
    "\377\000\060\377\000\377\000\000J\000\377\060\377\000J\000\377J\000\377\060\377\000\377\000\000\377"
    "\000\000\377\000\000J\000\377\060\377\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377"
    "\000\000J\000\377\060\377\000\060\377\000J\000\377\060\377\000\377\000\000\377\000\000\377\000\000J\000"
    "\377\060\377\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000J\000\377\060\377"
    "\000\377\000\000J\000\377J\000\377\060\377\000\377\000\000\377\000\000J\000\377\060\377\000\377\000\000"
    "\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000J\000\377\060\377\000\377\000\000\060\377\000"
    "J\000\377\060\377\000\377\000\000\377\000\000J\000\377\060\377\000\377\000\000\377\000\000\377\000\000"
    "\377\000\000\377\000\000\377\000\000J\000\377\060\377\000\377\000\000\060\377\000J\000\377J\000\377\060"
    "\377\000\377\000\000J\000\377J\000\377J\000\377J\000\377J\000\377J\000\377J\000\377\377\000\000J"
    "\000\377\060\377\000\377\000\000\377\000\000\060\377\000J\000\377\060\377\000\377\000\000J\000\377\060"
    "\377\000\060\377\000\060\377\000\060\377\000\060\377\000\060\377\000\377\000\000J\000\377\060\377"
    "\000\377\000\000\377\000\000\060\377\000J\000\377J\000\377\060\377\000J\000\377\060\377\000\377\000"
    "\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000J\000\377\060\377\000\377\000\000\377\000"
    "\000\377\000\000\060\377\000J\000\377\060\377\000J\000\377\060\377\000\377\000\000\377\000\000\377\000"
    "\000\377\000\000\377\000\000\377\000\000J\000\377\060\377\000\377\000\000\377\000\000\377\000\000\060\377"
    "\000J\000\377J\000\377J\000\377\060\377\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000"
    "\377\000\000J\000\377\060\377\000\377\000\000\377\000\000\377\000\000\377\000\000\060\377\000J\000\377"
    "J\000\377\060\377\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000J\000\377\060"
    "\377\000\377\000\000\377\000\000\377\000\000\377\000\000\060\377\000J\000\377J\000\377\060\377\000\377"
    "\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000J\000\377\060\377\000\377\000\000\377"
    "\000\000\377\000\000\377\000\000\377\000\000\060\377\000J\000\377J\000\377J\000\377J\000\377J\000\377"
    "J\000\377J\000\377\377\000\000\060\377\000\060\377\000\377\000\000\377\000\000\377\000\000\377\000\000"
    "\377\000\000\377\000\000\060\377\000\060\377\000\060\377\000\060\377\000\060\377\000\060\377\000\060"
    "\377\000";

static void _onresize(s32 w, s32 h) {
    if (w == 0 && h == 0) glfwGetWindowSize(rwin, &w, &h);

    rwidth = w;
    rheight = h;

    glViewport(0, 0, rwidth, rheight);
}

static void resize_callback(GLFWwindow *win, int w, int h) {
    _onresize(w, h);
}

void ZEScreen_init(u32 width, u32 height, f64 fov, const char *title, u32 flags) {
    if (glfwInit() == 0) ZELog(ZELOG_FATAL, "GLFW: Failed to initialize GLFW");

    glfwDefaultWindowHints();
    glfwWindowHint(GLFW_AUTO_ICONIFY, 0);

    rfov = fov;
    rwidth = width;
    rheight = height;
    GLFWmonitor *monitor = NULL;
    if (flags & ZEScreenFlag_Fullscreen) {
        monitor = glfwGetPrimaryMonitor();
    }
    if (flags & ZEScreenFlag_Resizeable) glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    else glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    if (flags & ZEScreenFlag_Borderless) ZELog(ZELOG_WARNING, "GLFW: Flag ZEScreenFlag_Borderless isn't avaliable on GLFW");
    rwin = glfwCreateWindow(width, height, title, monitor, NULL);
    if (rwin == NULL) ZELog(ZELOG_FATAL, "GLFW: Failed to open a window");

    _onresize(0, 0);

    glfwSetWindowSizeCallback(rwin, resize_callback);
    glfwSetKeyCallback(rwin, key_callback);
    glfwMakeContextCurrent(rwin);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
}

static bool is_closed = false;
static bool isnt_closed = true;

bool ZEScreen_IsClosed() {
    return is_closed;
}

bool ZEScreen_IsNtClosed() {
    return isnt_closed;
}

void ZEScreen_BeginFrame(f64 *omx, f64 *omy) {
    for (int i = 0; i < 1024; ++i) {
        if (key_status[i] & KEYST_PRESSED) {
            key_status[i] &= ~KEYST_PRESSED;
            key_status[i] |= KEYST_DOWN;
        }
        if (key_status[i] & KEYST_RELEASED) {
            key_status[i] &= ~(KEYST_RELEASED | KEYST_DOWN);
        }
    }

    is_closed = glfwWindowShouldClose(rwin);
    isnt_closed = !is_closed;
    if (is_closed) return;
    glfwPollEvents();

    glClearColor(0.f, 1.f, 1.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ZEScreen_ResetCamera();

    // It doesn't work on MS Windows.
    if (omx) *omx = 0;
    if (omy) *omy = 0;
    // i32 mx_ = 0, my_ = 0;
    // RGFW_window_getMouse(rwin, &mx_, &my_);
    // {
    //     f32 mx = mx_;
    //     f32 my = my_;
    //     mx -= rwidth*.5;
    //     my -= rheight*.5;
    //     mx /= rwidth*.5;
    //     my /= rheight*.5;
    //     if (omx) *omx = mx;
    //     if (omy) *omy = my;
    //     RGFW_window_moveMouse(rwin, rwin->x+rwidth*.5, rwin->y+rheight*.5);
    // }
}

static ZEVec3 rcampos = {0};

void ZEScreen_ResetCamera() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    f64 aspect = ((f64)rwidth)/((f64)rheight);
    { // stolen from GLu.
        f32 m[4][4] = {0};
        f32 radians = rfov / 2 * PI / 180;
        f32 zNear = .01, zFar = 100.;
        f32 deltaZ = zFar-zNear;
        f32 sine = sin(radians);
        f32 cotangent = cos(radians)/sine;
        m[0][0] = cotangent / aspect;
        m[1][1] = cotangent;
        m[2][2] = -(zFar + zNear) / deltaZ;
        m[2][3] = -1;
        m[3][2] = -2 * zNear * zFar / deltaZ;

        glLoadMatrixf((f32 *)m);
    }
    {
        f32 m[16] = {0};
        m[0] = 1.f;
        m[5] = 1.f;
        m[10] = -1.f;
        m[15] = 1.f;
        glMultMatrixf(m);
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    memset(&rcampos, 0, sizeof(rcampos));
}

void ZEScreen_TranslateCamera(ZEVec3 origin) {
    glMatrixMode(GL_PROJECTION);
    glTranslated(-origin.x, -origin.y, -origin.z);
    rcampos = origin;
}

void ZEScreen_RotateCamera(f64 yaw, f64 pitch, f64 roll) {
    glMatrixMode(GL_PROJECTION);
    glRotated(roll*RAD2DEG, 0, 0, 1);
    glRotated(pitch*RAD2DEG, 1, 0, 0);
    glRotated(yaw*RAD2DEG, 0, 1, 0);
}

void ZEScreen_EndFrame() {
    glfwSwapBuffers(rwin);
    glFlush();
}

void ZEScreen_DrawCircle(ZEVec3 o, f64 r, ZEColor col) {
    glBegin(GL_TRIANGLES);
    glColor4d(col.r, col.g, col.b, col.a);

#define CIRCLE_SEGMENTS 16
    f64 lx = 0, ly = r;
    for (f64 i = 0; i <= TAU; i += TAU/CIRCLE_SEGMENTS) {
        f64 cx = sin(i)*r,
            cy = cos(i)*r;
        glVertex3d(o.x, o.y, o.z);
        glVertex3d(o.x+lx, o.y+ly, o.z);
        glVertex3d(o.x+cx, o.y+cy, o.z);
        lx = cx;
        ly = cy;
    }

    glVertex3d(o.x, o.y, o.z);
    glVertex3d(o.x+lx, o.y+ly, o.z);
    glVertex3d(o.x, o.y+r, o.z);

    glEnd();
}

#define CUT_DST 450

void ZEScreen_DrawTriangleRaw(ZEVec3 a, ZEVec3 b, ZEVec3 c, ZEColor col) {
    if (abs(a.x-rcampos.x)+abs(a.y-rcampos.y)+abs(a.z-rcampos.z) >= CUT_DST &&
        abs(b.x-rcampos.x)+abs(b.y-rcampos.y)+abs(b.z-rcampos.z) >= CUT_DST &&
        abs(c.x-rcampos.x)+abs(c.y-rcampos.y)+abs(c.z-rcampos.z) >= CUT_DST) return;
    glBegin(GL_TRIANGLES);
    glColor4d(col.r, col.g, col.b, col.a);
    glVertex3d(a.x, a.y, a.z);
    glVertex3d(b.x, b.y, b.z);
    glVertex3d(c.x, c.y, c.z);
    glEnd();
}

static f64 get_light(ZEVec3 a, ZEVec3 b, ZEVec3 c) {
    f64 ax = b.x-a.x,     ay = b.y-a.y,     az = b.z-a.z;
    f64 bx = c.x-a.x,     by = c.y-a.y,     bz = c.z-a.z;
    f64 nx = ay*bz-az*by, ny = az*bx-ax*bz, nz = ax*by-ay*bx;
    f64 nm = 1/sqrt(nx*nx+ny*ny+nz*nz);
    nx *= nm; ny *= nm; nz *= nm;
    f64 d = DOT3(nx, ny, nz, 0.07053, 0.21821, 0.70534);
    if (d < 0) d = 0;
    d += .3f;
    if (d > 1) d = 1;
    return d;
}

void ZEScreen_DrawTriangle(ZEVec3 a, ZEVec3 b, ZEVec3 c, ZEColor col) {
    if (abs(a.x-rcampos.x)+abs(a.y-rcampos.y)+abs(a.z-rcampos.z) >= CUT_DST &&
        abs(b.x-rcampos.x)+abs(b.y-rcampos.y)+abs(b.z-rcampos.z) >= CUT_DST &&
        abs(c.x-rcampos.x)+abs(c.y-rcampos.y)+abs(c.z-rcampos.z) >= CUT_DST) return;
    f64 d = get_light(a, b, c);
    glBegin(GL_TRIANGLES);
    glColor4d(col.r*d, col.g*d, col.b*d, col.a);
    glVertex3d(a.x, a.y, a.z);
    glVertex3d(b.x, b.y, b.z);
    glVertex3d(c.x, c.y, c.z);
    glEnd();
}

void ZEScreen_DrawTriangle_Ex(ZEVec3 a, ZEVec3 b, ZEVec3 c, ZEColor a_c, ZEColor b_c, ZEColor c_c) {
    if (abs(a.x-rcampos.x)+abs(a.y-rcampos.y)+abs(a.z-rcampos.z) >= CUT_DST &&
        abs(b.x-rcampos.x)+abs(b.y-rcampos.y)+abs(b.z-rcampos.z) >= CUT_DST &&
        abs(c.x-rcampos.x)+abs(c.y-rcampos.y)+abs(c.z-rcampos.z) >= CUT_DST) return;
    f64 d = get_light(a, b, c);
    glBegin(GL_TRIANGLES);
    glColor4d(a_c.r*d, a_c.g*d, a_c.b*d, a_c.a);
    glVertex3d(a.x, a.y, a.z);
    glColor4d(b_c.r*d, b_c.g*d, b_c.b*d, b_c.a);
    glVertex3d(b.x, b.y, b.z);
    glColor4d(c_c.r*d, c_c.g*d, c_c.b*d, c_c.a);
    glVertex3d(c.x, c.y, c.z);
    glEnd();
}

bool ZEScreen_IsKeyPressed(ZEKey key) {
    return key_status[key_mapping[key]] & KEYST_PRESSED;
}

bool ZEScreen_IsKeyDown(ZEKey key) {
    return key_status[key_mapping[key]] & KEYST_DOWN;
}

bool ZEScreen_IsKeyReleased(ZEKey key) {
    return key_status[key_mapping[key]] & KEYST_RELEASED;
}

static bool _depth_test = true;

bool ZEScreen_GetDepthTest() {
    return _depth_test;
}

void ZEScreen_SetDepthTest(bool test) {
    _depth_test = test;
    glDepthFunc(test ? GL_LESS : GL_ALWAYS);
}

void ZEScreen_RenderModel(ZEModel model, ZETransformW transform) {
    for (size_t i = 0; i < model.face_count; ++i) {
        ZEFace face = model.faces[i];
        ZEScreen_DrawTriangle_Ex(
            ZETransformW_Apply(transform, model.verteces[face.a]),
            ZETransformW_Apply(transform, model.verteces[face.b]),
            ZETransformW_Apply(transform, model.verteces[face.c]),
            model.colors[face.a],
            model.colors[face.b],
            model.colors[face.c]
            );
    }
}

void *ZEScreen_GetSystemHandler() {
    return rwin;
}
