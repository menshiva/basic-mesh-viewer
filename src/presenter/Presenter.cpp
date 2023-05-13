#include "Presenter.hpp"
#include <cstdio>
#include "../config.hpp"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>

EM_BOOL EmscriptenWindowResizedCallback(const int, const void*, void *PresenterPtr) {
    double Width, Height;
    emscripten_get_element_css_size("canvas", &Width, &Height);
    const int w = (int) Width, h = (int) Height;
    glfwSetWindowSize(((Presenter*) PresenterPtr)->GetWindow(), w, h);
    return true;
}
#endif

Presenter::Presenter() : m_pWindow(nullptr) {}

Presenter::~Presenter() {
    m_pOnDestroy();
    glfwDestroyWindow(m_pWindow);
    glfwTerminate();
}

Presenter &Presenter::WithOnInitCallback(std::function<bool(GLFWwindow*)> &&Callback) {
    m_pOnInit = Callback;
    return *this;
}

Presenter &Presenter::WithOnUpdateCallback(std::function<void()> &&Callback) {
    m_pOnUpdate = Callback;
    return *this;
}

Presenter &Presenter::WithOnDrawCallback(std::function<void()> &&Callback) {
    m_pOnDraw = Callback;
    return *this;
}

Presenter &Presenter::WithOnDestroyCallback(std::function<void()> &&Callback) {
    m_pOnDestroy = Callback;
    return *this;
}

int Presenter::InitGlfwCreateWindowAndLoop() {
    glfwSetErrorCallback([] (const int error, const char *description) {
        fprintf(stderr, "GLFW Error %d: %s.\n", error, description);
    });
    if (!glfwInit())
        return 1;

    m_pWindow = InitWindow(this);
    if (!m_pWindow)
        return 1;

    glfwMakeContextCurrent(m_pWindow);
    glfwSwapInterval(1); // Enable vsync

    if (!m_pOnInit(m_pWindow))
        return 1;

#ifndef __EMSCRIPTEN__
    while (!glfwWindowShouldClose(m_pWindow))
        Tick(this);
#else
    emscripten_set_window_title(Config::WIN_TITLE);

    if constexpr (Config::WIN_DEFAULT_MAXIMIZED) {
        EmscriptenFullscreenStrategy Strategy;
	    Strategy.scaleMode = EMSCRIPTEN_FULLSCREEN_CANVAS_SCALE_STDDEF;
	    Strategy.filteringMode = EMSCRIPTEN_FULLSCREEN_FILTERING_DEFAULT;
	    Strategy.canvasResizedCallback = EmscriptenWindowResizedCallback;
        Strategy.canvasResizedCallbackUserData = this;
	    emscripten_enter_soft_fullscreen("canvas", &Strategy);
    }

    emscripten_set_main_loop_arg(Presenter::Tick, this, 0, true);
#endif

    return 0;
}

GLFWwindow *Presenter::InitWindow(Presenter *UserPointer) {
#ifndef __EMSCRIPTEN__
    // TODO
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    if constexpr (Config::WIN_DEFAULT_MAXIMIZED)
        glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
#else
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#endif

    const auto Window = glfwCreateWindow(
        Config::WIN_DEFAULT_WIDTH, Config::WIN_DEFAULT_HEIGHT,
        Config::WIN_TITLE,
        nullptr, nullptr
    );
    if (!Window)
        return nullptr;

#ifndef __EMSCRIPTEN__
    glfwSetWindowSizeLimits(
        Window,
        Config::WIN_MIN_WIDTH, Config::WIN_MIN_HEIGHT,
        GLFW_DONT_CARE, GLFW_DONT_CARE
    );

    glfwSetWindowUserPointer(Window, UserPointer);
    glfwSetFramebufferSizeCallback(Window, [] (GLFWwindow *Window, int, int) {
        Presenter::Tick((Presenter*) glfwGetWindowUserPointer(Window));
    });
#endif

    return Window;
}

void Presenter::Tick(void *PresenterPtr) {
    const auto &P = *((Presenter*) PresenterPtr);

    P.m_pOnUpdate();

    int w, h;
    glfwGetFramebufferSize(P.m_pWindow, &w, &h);
    glViewport(0, 0, w, h);

    glClearColor(Config::BACKGROUND_COLOR[0], Config::BACKGROUND_COLOR[1], Config::BACKGROUND_COLOR[2], 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    P.m_pOnDraw();

    glfwSwapBuffers(P.m_pWindow);
    glfwPollEvents();
}
