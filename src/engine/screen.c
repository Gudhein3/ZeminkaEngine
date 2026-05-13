// TODO: Move multi-threading logic to a separated module.
#include <pthread.h>
#include <errno.h>
#include <unistd.h>

#include <zeminka/engine.h>
#include <stdio.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

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

// https://www.khronos.org/assets/uploads/books/openglr_es_20_programming_guide_sample.pdf
static GLuint LoadShader(GLenum type, const char *shaderSrc) {
    GLuint shader;
    GLint compiled;
    shader = glCreateShader(type);
    if(shader == 0)
        return 0;
    glShaderSource(shader, 1, &shaderSrc, NULL);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

    if(!compiled) {
        GLint infoLen = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
        if(infoLen > 1) {
            char* infoLog = malloc(sizeof(char) * infoLen);
            glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
            ZELog(ZELOG_WARNING, "OpenGL: Error compiling shader:\n%s\n%s\n", infoLog, shaderSrc);
            free(infoLog);
        }
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

// https://www.khronos.org/assets/uploads/books/openglr_es_20_programming_guide_sample.pdf
u64 ZEScreen_CompileShader(ZEShaderProg *prog, const char *vert, const char *frag) {
    prog->vert = LoadShader(GL_VERTEX_SHADER, vert);
    prog->frag = LoadShader(GL_FRAGMENT_SHADER, frag);
    prog->prog = glCreateProgram();
    if (prog->prog == 0) return 0;
    glAttachShader(prog->prog, prog->vert);
    glAttachShader(prog->prog, prog->frag);
    glBindAttribLocation(prog->prog, 0, "vPosition");
    glLinkProgram(prog->prog);
    int linked;
    glGetProgramiv(prog->prog, GL_LINK_STATUS, &linked);
    if(!linked)
    {
        GLint infoLen = 0;
        glGetProgramiv(prog->prog, GL_INFO_LOG_LENGTH, &infoLen);
        if(infoLen > 1) {
            char* infoLog = malloc(sizeof(char) * infoLen);
            glGetProgramInfoLog(prog->prog, infoLen, NULL, infoLog);
            ZELog(ZELOG_WARNING, "OpenGL: Error linking program:\n%s\n", infoLog);
            free(infoLog);
        }
        glDeleteProgram(prog->prog);
        return 0;
    }
    return prog->prog;
}

static GLuint rrendtex = -1;
static GLuint rrenddtex = -1;
static GLuint rrendfbo = -1;

void ZEScreen_DrawShaderTriangle(ZEShaderProg *prog, ZEVec2 a, ZEVec2 b, ZEVec2 c) {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glUseProgram(prog->prog);
    GLfloat verts[] = {
        a.x,a.y,
        b.x,b.y,
        c.x,c.y
    };
    GLuint vbo;
    glGenBuffers(1, &vbo);
    GLint uSysTime = glGetUniformLocation(prog->prog, "uSysTime");
    glUniform1f(uSysTime, ZEsystemTime);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, rrendtex);
    GLint uTex0 = glGetUniformLocation(prog->prog, "screen");
    glUniform1i(uTex0, 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, rrenddtex);
    GLint uTex1 = glGetUniformLocation(prog->prog, "screen_depth");
    glUniform1i(uTex1, 1);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
    GLint locPos = glGetAttribLocation(prog->prog, "vPosition");
    glEnableVertexAttribArray(locPos);
    glVertexAttribPointer(locPos, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 3);

    glDisableVertexAttribArray(locPos);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &vbo);
    glUseProgram(0);
}

void ZEScreen_Shader_SetUniformf(ZEShaderProg *prog, const char *name, f64 param) {
    glUseProgram(prog->prog);
    GLint uSysTime = glGetUniformLocation(prog->prog, name);
    glUniform1f(uSysTime, param);
    glUseProgram(0);
}

void ZEScreen_Shader_SetUniformi(ZEShaderProg *prog, const char *name, int param) {
    glUseProgram(prog->prog);
    GLint uSysTime = glGetUniformLocation(prog->prog, name);
    glUniform1i(uSysTime, param);
    glUseProgram(0);
}

void ZEScreen_Shader_SetUniformf2(ZEShaderProg *prog, const char *name, ZEVec2 param) {
    glUseProgram(prog->prog);
    GLint uSysTime = glGetUniformLocation(prog->prog, name);
    glUniform2f(uSysTime, param.x, param.y);
    glUseProgram(0);
}

void ZEScreen_Shader_SetUniformf3(ZEShaderProg *prog, const char *name, ZEVec3 param) {
    glUseProgram(prog->prog);
    GLint uSysTime = glGetUniformLocation(prog->prog, name);
    glUniform3f(uSysTime, param.x, param.y, param.z);
    glUseProgram(0);
}

void ZEScreen_Shader_SetUniformf4(ZEShaderProg *prog, const char *name, ZEVec4 param) {
    glUseProgram(prog->prog);
    GLint uSysTime = glGetUniformLocation(prog->prog, name);
    glUniform4f(uSysTime, param.x, param.y, param.z, param.w);
    glUseProgram(0);
}

static void _onresize(s32 w, s32 h) {
    if (w == 0 && h == 0) glfwGetWindowSize(rwin, &w, &h);

    rwidth = w;
    rheight = h;

    glViewport(0, 0, rwidth, rheight);

    if (rrendfbo != -1) glDeleteFramebuffers(1, &rrendfbo);
    if (rrendtex != -1) glDeleteTextures(1, &rrendtex);
    if (rrenddtex != -1) glDeleteTextures(1, &rrenddtex);

	glGenFramebuffers(1, &rrendfbo);

	glGenTextures(1, &rrendtex);
	glGenTextures(1, &rrenddtex);

	glBindFramebuffer(GL_FRAMEBUFFER, rrendfbo);

	glBindTexture(GL_TEXTURE_2D, rrendtex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_2D, rrenddtex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, w, h, 0,
        GL_DEPTH_COMPONENT, GL_FLOAT, NULL
    );

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, rrenddtex, 0);

	glBindTexture(GL_TEXTURE_2D, rrendtex);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, rrendtex, 0);

}

static void resize_callback(GLFWwindow *win, int w, int h) {
    _onresize(w, h);
}

typedef enum {
    RTK_LINE      = 0,
    RTK_TRI       = 1,
    RTK_TRI_FANCY = 2,
    RTK_TRIEX     = 3
} RenderTaskKind;

typedef struct {
    RenderTaskKind kind;
    ZEVec3 a, b, c;
    ZEColor x1, x2, x3;
    int do_render;
} RenderTask;

typedef struct {
    RenderTask *items;
    size_t count, capacity;
} RenderTasks;

typedef struct {
    RenderTask *first;
    size_t count;
    int thread_id; // Shouldn't be changed after initialization
    int locked;
} RenderThreadArg;

// TODO: Ensure that rendering will be performed on the same state as at moment of requiring.

// Maximum supported amount of threads that game engine can use
#define MAX_THREADS 64

static RenderTasks rtks = {0};
static pthread_t threads[MAX_THREADS] = {0};
static RenderThreadArg tdata[MAX_THREADS] = {0};
static pthread_mutex_t rtks_mutex[MAX_THREADS] = {0};
static long thread_count;

static void *render_thread(void *arg);

static void get_thread_cnt() {
    thread_count = sysconf(_SC_NPROCESSORS_ONLN);
    if (thread_count >= MAX_THREADS) thread_count = MAX_THREADS;
    else if (thread_count >= 8) thread_count--; // Giving a thread to others.
    else if (thread_count >= 1) /*thread_count = thread_count*/;
}

void ZEScreen_init(u32 width, u32 height, f64 fov, const char *title, u32 flags) {
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setstacksize(&attr, 512);
    get_thread_cnt();
    //pthread_mutex_init(&rtks_wakeup_mutex, NULL);
    //pthread_mutex_lock(&rtks_wakeup_mutex);
    for (int i = 0; i < thread_count; ++i) {
        //rtks_mutex[i] = PTHREAD_MUTEX_INITIALIZER;
        //rtks_complete_sig[i] = PTHREAD_COND_INITIALIZER;
        pthread_mutex_init(&rtks_mutex[i], NULL);
        pthread_mutex_lock(&rtks_mutex[i]);
        tdata[i].thread_id = i;
        pthread_create(&threads[i], &attr, render_thread, &tdata[i]);
    }
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
    if (flags & ZEScreenFlag_Borderless) ZELog(ZELOG_WARNING, "GLFW: Flag ZEScreenFlag_Borderless isn't avaliable in GLFW");
    rwin = glfwCreateWindow(width, height, title, monitor, NULL);
    if (rwin == NULL) ZELog(ZELOG_FATAL, "GLFW: Failed to open a window");
    glfwSetWindowSizeCallback(rwin, resize_callback);
    glfwSetKeyCallback(rwin, key_callback);
    glfwMakeContextCurrent(rwin);
    gladLoadGL();
    glfwSetInputMode(rwin, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    _onresize(0, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, rrendfbo);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glDepthFunc(GL_LESS);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
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

    if (omx) *omx = 0;
    if (omy) *omy = 0;
    static f64 smx_ = 0, smy_ = 0;
    f64 mx_ = 0, my_ = 0;
    glfwGetCursorPos(rwin, &mx_, &my_);
    {
        f32 mx = mx_;
        f32 my = my_;
        mx -= rwidth*.5;
        my -= rheight*.5;
        mx /= rwidth*.5;
        my /= rheight*.5;
        if (omx) *omx = mx-smx_;
        if (omy) *omy = my-smy_;
        smx_ = mx;
        smy_ = my;
    }
}

static ZEVec3     rcampos = {0};
static ZEGeomBBox rcambox = {0};

void ZEScreen_ResetCamera() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    f64 aspect = ((f64)rwidth)/((f64)rheight);
    { // stolen from GLu.
        f32 m[4][4] = {0};
        f32 radians = rfov / 2 * PI / 180;
        f32 zNear = .01, zFar = 100000.;
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
    memset(&rcambox, 0, sizeof(rcambox));
}

#define CUT_DST 1000

void ZEScreen_TranslateCamera(ZEVec3 origin) {
    glMatrixMode(GL_PROJECTION);
    glTranslated(-origin.x, -origin.y, -origin.z);
    rcampos = origin;
    rcambox.center = rcampos;
    rcambox.dimensions = ZEVec3_From1(CUT_DST);
}

void ZEScreen_RotateCamera(f64 yaw, f64 pitch, f64 roll) {
    glMatrixMode(GL_PROJECTION);
    glRotated(roll*RAD2DEG, 0, 0, 1);
    glRotated(pitch*RAD2DEG, 1, 0, 0);
    glRotated(yaw*RAD2DEG, 0, 1, 0);
}

static f64 get_light(ZEVec3 a, ZEVec3 b, ZEVec3 c, ZEVec3 sun) {
    f64 ax = b.x-a.x,     ay = b.y-a.y,     az = b.z-a.z;
    f64 bx = c.x-a.x,     by = c.y-a.y,     bz = c.z-a.z;
    f64 nx = ay*bz-az*by, ny = az*bx-ax*bz, nz = ax*by-ay*bx;
    f64 nm = 1/sqrt(nx*nx+ny*ny+nz*nz);
    nx *= nm; ny *= nm; nz *= nm;
    f64 d = DOT3(nx, ny, nz, sun.x, sun.y, sun.z);
    if (d < 0) d = 0;
    d += .3f;
    if (d > 1) d = 1;
    return d;
}

static void *render_thread(void *arg) {
    if (arg == NULL) return NULL;
    RenderThreadArg *rta = arg;
    for (;;) {
        while (rta->locked == 1) {}
        pthread_mutex_lock(&rtks_mutex[rta->thread_id]);
        f64 t = ZE_getSystemTime();
        const f64 sp = 1/60;
        ZEVec3 sun = ZEVec3_Norm(ZEVec3_From3(cos(ZEsystemTime*sp), sin(ZEsystemTime*sp), cos(ZEsystemTime/TAU*.01*sp)));
        // ZEVec3 sun = ZEVec3_Norm(ZEVec3_From3(1.,-1.,1.));
        for (size_t i = 0; i < rta->count; ++i) {
            RenderTask *task = &rta->first[i];
            task->do_render = (ZEGeomIsPointInBBox(task->a, rcambox) || ZEGeomIsPointInBBox(task->b, rcambox) || ZEGeomIsPointInBBox(task->c, rcambox)) &&
                              (ZEVec3_Dist(task->a, rcampos)<CUT_DST || ZEVec3_Dist(task->b, rcampos)<CUT_DST || ZEVec3_Dist(task->c, rcampos)<CUT_DST);
            if (task->do_render && task->kind == RTK_TRI_FANCY) {
                f64 light = get_light(task->a, task->b, task->c, sun);
                task->x1 = ZEColor_scale(task->x1, light);
                task->x2 = ZEColor_scale(task->x2, light);
                task->x3 = ZEColor_scale(task->x3, light);
            }
        }
        rta->locked = 1;
        printf("Imta %dth renderer; All that nonsence took %2.8fs\n", rta->thread_id, ZE_getSystemTime()-t);
        pthread_mutex_unlock(&rtks_mutex[rta->thread_id]);
    }
    return NULL;
}

void ZEScreen_EndFrame() {
	glBindFramebuffer(GL_FRAMEBUFFER, rrendfbo);
    glClearColor(0.f, 1.f, 1.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBegin(GL_TRIANGLES);
    if (rtks.count > 0) {
        for (int i = 0, ti = 0; i <= rtks.count; ++i) {
            if (ti*thread_count/rtks.count != i*thread_count/rtks.count || i == rtks.count) {
                int t = ti*thread_count/rtks.count;
                tdata[t].first = &rtks.items[ti];
                tdata[t].count = i-ti;
                pthread_mutex_unlock(&rtks_mutex[t]);
                ti = i;
            }
        }
        for (int i = 0; i < thread_count; ++i) {
            pthread_mutex_lock(&rtks_mutex[i]);
            tdata[i].locked = 0;
        }
    }
    int trc = 0;
    da_foreach(&rtks, RenderTask, task) {
        if (!task->do_render) {
            continue;
        }
        trc++;
        switch (task->kind) {
        case RTK_TRI:
            glColor4d(task->x1.r, task->x1.g, task->x1.b, task->x1.a);
            glVertex3d(task->a.x, task->a.y, task->a.z);
            glVertex3d(task->b.x, task->b.y, task->b.z);
            glVertex3d(task->c.x, task->c.y, task->c.z);
            break;
        case RTK_TRI_FANCY:
            glColor4d(task->x1.r, task->x1.g, task->x1.b, task->x1.a);
            glVertex3d(task->a.x, task->a.y, task->a.z);
            glColor4d(task->x2.r, task->x2.g, task->x2.b, task->x2.a);
            glVertex3d(task->b.x, task->b.y, task->b.z);
            glColor4d(task->x3.r, task->x3.g, task->x3.b, task->x3.a);
            glVertex3d(task->c.x, task->c.y, task->c.z);
            break;
        case RTK_TRIEX:
            glColor4d(task->x1.r, task->x1.g, task->x1.b, task->x1.a);
            glVertex3d(task->a.x, task->a.y, task->a.z);
            glColor4d(task->x2.r, task->x2.g, task->x2.b, task->x2.a);
            glVertex3d(task->b.x, task->b.y, task->b.z);
            glColor4d(task->x3.r, task->x3.g, task->x3.b, task->x3.a);
            glVertex3d(task->c.x, task->c.y, task->c.z);
            break;
        default:
            assert(0 && "Unreachable");
        }
    }
    printf("We have %d real triangles\n", trc);
    glEnd();
    rtks.count = 0;
    glFlush();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glfwSwapBuffers(rwin);
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

void ZEScreen_DrawTriangleRaw(ZEVec3 a, ZEVec3 b, ZEVec3 c, ZEColor col) {
    if (!ZEGeomIsPointInBBox(a, rcambox) || !ZEGeomIsPointInBBox(b, rcambox) || !ZEGeomIsPointInBBox(c, rcambox)) return;
    RenderTask task;
    task.kind = RTK_TRI;
    task.a = a;
    task.b = b;
    task.c = c;
    task.x1 = col;
    task.x2 = col;
    task.x3 = col;
    task.do_render = false;
    da_append(&rtks, task);
}

void ZEScreen_DrawTriangle(ZEVec3 a, ZEVec3 b, ZEVec3 c, ZEColor col) {
    if (!ZEGeomIsPointInBBox(a, rcambox) || !ZEGeomIsPointInBBox(b, rcambox) || !ZEGeomIsPointInBBox(c, rcambox)) return;
    RenderTask task;
    task.kind = RTK_TRI_FANCY;
    task.a = a;
    task.b = b;
    task.c = c;
    task.x1 = col;
    task.x2 = col;
    task.x3 = col;
    task.do_render = false;
    da_append(&rtks, task);
}

void ZEScreen_DrawTriangle_Ex(ZEVec3 a, ZEVec3 b, ZEVec3 c, ZEColor a_c, ZEColor b_c, ZEColor c_c) {
    if (!ZEGeomIsPointInBBox(a, rcambox) || !ZEGeomIsPointInBBox(b, rcambox) || !ZEGeomIsPointInBBox(c, rcambox)) return;
    RenderTask task;
    task.kind = RTK_TRIEX;
    task.a = a;
    task.b = b;
    task.c = c;
    task.x1 = a_c;
    task.x2 = b_c;
    task.x3 = c_c;
    task.do_render = false;
    da_append(&rtks, task);
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
