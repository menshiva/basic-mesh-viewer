#include "Presenter.hpp"
#include <cstdio>
#include "../config.hpp"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>

EM_BOOL EmscriptenWindowResizedCallback(const int, const void*, void *presenterPtr) {
    double width, height;
    emscripten_get_element_css_size("canvas", &width, &height);
    glfwSetWindowSize(((Presenter*) presenterPtr)->GetWindow(), (int) width, (int) height);
    return true;
}
#endif

Presenter::Presenter() : m_pWindow(nullptr) {}

Presenter::~Presenter() {
    m_pOnDestroy();
    glfwDestroyWindow(m_pWindow);
    glfwTerminate();
}

Presenter &Presenter::WithOnInitCallback(std::function<bool(GLFWwindow*)> &&callback) {
    m_pOnInit = callback;
    return *this;
}

Presenter &Presenter::WithOnUpdateCallback(std::function<void()> &&callback) {
    m_pOnUpdate = callback;
    return *this;
}

Presenter &Presenter::WithOnDrawCallback(std::function<void()> &&callback) {
    m_pOnDraw = callback;
    return *this;
}

Presenter &Presenter::WithOnDestroyCallback(std::function<void()> &&callback) {
    m_pOnDestroy = callback;
    return *this;
}

int Presenter::InitGlfwCreateWindowAndLoop() {
    glfwSetErrorCallback([] (const int error, const char *description) {
        fprintf(stderr, "GLFW Error %d: %s\n", error, description);
        exit(1);
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
        EmscriptenFullscreenStrategy strategy;
	    strategy.scaleMode = EMSCRIPTEN_FULLSCREEN_CANVAS_SCALE_STDDEF;
	    strategy.filteringMode = EMSCRIPTEN_FULLSCREEN_FILTERING_DEFAULT;
	    strategy.canvasResizedCallback = EmscriptenWindowResizedCallback;
        strategy.canvasResizedCallbackUserData = this;
	    emscripten_enter_soft_fullscreen("canvas", &strategy);
    }

    emscripten_set_main_loop_arg(Presenter::Tick, this, 0, true);
#endif

    return 0;
}

GLFWwindow *Presenter::InitWindow(Presenter *userPointer) {
#ifndef __EMSCRIPTEN__
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if !NDEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif
    if constexpr (Config::WIN_DEFAULT_MAXIMIZED)
        glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
#else
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#endif

    const auto window = glfwCreateWindow(
        Config::WIN_DEFAULT_WIDTH, Config::WIN_DEFAULT_HEIGHT,
        Config::WIN_TITLE,
        nullptr, nullptr
    );
    if (!window)
        return nullptr;

#ifndef __EMSCRIPTEN__
    glfwSetWindowSizeLimits(
        window,
        Config::WIN_MIN_WIDTH, Config::WIN_MIN_HEIGHT,
        GLFW_DONT_CARE, GLFW_DONT_CARE
    );

    glfwSetWindowUserPointer(window, userPointer);
    glfwSetFramebufferSizeCallback(window, [] (GLFWwindow *window, int, int) {
        Presenter::Tick((Presenter*) glfwGetWindowUserPointer(window));
    });
#endif

    return window;
}

void Presenter::Tick(void *presenterPtr) {
    const auto &presenter = *((Presenter*) presenterPtr);

    glfwPollEvents();
    presenter.m_pOnUpdate();

    int w, h;
    glfwGetFramebufferSize(presenter.m_pWindow, &w, &h);
    glViewport(0, 0, w, h);

    glClearColor(Config::BACKGROUND_COLOR.r, Config::BACKGROUND_COLOR.g, Config::BACKGROUND_COLOR.b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    presenter.m_pOnDraw();

    glfwSwapBuffers(presenter.m_pWindow);
}
