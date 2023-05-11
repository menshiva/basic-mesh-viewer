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

Presenter &Presenter::WithOnInitCallback(std::function<void(const char*, GLFWwindow*)> &&Callback) {
    m_pOnInit = Callback;
    return *this;
}

Presenter &Presenter::WithOnPreRenderCallback(std::function<void()> &&Callback) {
    m_pOnPreRender = Callback;
    return *this;
}

Presenter &Presenter::WithOnRenderCallback(std::function<void()> &&Callback) {
    m_pOnRender = Callback;
    return *this;
}

Presenter &Presenter::WithOnDestroyCallback(std::function<void()> &&Callback) {
    m_pOnDestroy = Callback;
    return *this;
}

void Presenter::InitGlfwCreateWindowAndLoop() {
    // GLFW Init
    glfwSetErrorCallback([] (const int error, const char *description) {
        fprintf(stderr, "GLFW Error %d: %s\n", error, description);
    });
    if (!glfwInit())
        return;

    // Decide GL+GLSL versions
#ifdef __EMSCRIPTEN__
    const char *GlslVersion = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#else
    const char *GlslVersion = "#version 130";
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    if constexpr (Config::WIN_DEFAULT_MAXIMIZED)
        glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
#endif

    // Create window with graphics context
    m_pWindow = glfwCreateWindow(
        Config::WIN_DEFAULT_WIDTH, Config::WIN_DEFAULT_HEIGHT,
        Config::WIN_TITLE,
        nullptr, nullptr
    );
    if (!m_pWindow)
        return;

#ifndef __EMSCRIPTEN__
    glfwSetWindowSizeLimits(
        m_pWindow,
        Config::WIN_DEFAULT_WIDTH, Config::WIN_DEFAULT_HEIGHT,
        GLFW_DONT_CARE, GLFW_DONT_CARE
    );

    glfwSetWindowUserPointer(m_pWindow, this);
    glfwSetFramebufferSizeCallback(m_pWindow, [] (GLFWwindow *window, int, int) {
        Presenter::Tick((Presenter*) glfwGetWindowUserPointer(window));
    });
#endif

    glfwMakeContextCurrent(m_pWindow);
    glfwSwapInterval(1); // Enable vsync

    m_pOnInit(GlslVersion, m_pWindow);

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
}

void Presenter::Tick(void *PresenterPtr) {
    const auto &P = *((Presenter*) PresenterPtr);

    P.m_pOnPreRender();

    int w, h;
    glfwGetFramebufferSize(P.m_pWindow, &w, &h);
    glViewport(0, 0, w, h);

    glClearColor(
        Config::BACKGROUND_COLOR[0], Config::BACKGROUND_COLOR[1],
        Config::BACKGROUND_COLOR[2], Config::BACKGROUND_COLOR[3]
    );
    glClear(GL_COLOR_BUFFER_BIT);

    P.m_pOnRender();

    glfwSwapBuffers(P.m_pWindow);
    glfwPollEvents();
}
