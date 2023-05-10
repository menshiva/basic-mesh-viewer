#include "Presenter.hpp"
#include <cstdio>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>

EM_BOOL emscriptenWindowResizedCallback(const int, const void*, void *presenterPtr) {
    double width, height;
    emscripten_get_element_css_size("canvas", &width, &height);
    const int w = (int) width, h = (int) height;
    glfwSetWindowSize(((Presenter*) presenterPtr)->getWindow(), w, h);
    return true;
}
#endif

Presenter::Presenter() : m_pWindow(nullptr) {}

Presenter::~Presenter() {
    if (m_pOnDestroy)
        m_pOnDestroy(*this);
    glfwDestroyWindow(m_pWindow);
    glfwTerminate();
}

Presenter &Presenter::withOnCreateCallback(std::function<void(const char*, const Presenter&)> &&Callback) {
    m_pOnCreate = Callback;
    return *this;
}

Presenter &Presenter::withOnTickCallback(std::function<void(const Presenter&)> &&Callback) {
    m_pOnTick = Callback;
    return *this;
}

Presenter &Presenter::withOnDestroyCallback(std::function<void(const Presenter&)> &&Callback) {
    m_pOnDestroy = Callback;
    return *this;
}

void Presenter::initGlfwCreateWindowAndLoop(
    const int defaultWidth, const int defaultHeight,
    const char *title,
    const bool fullscreen
) {
    // GLFW Init
    glfwSetErrorCallback([] (const int error, const char *description) {
        fprintf(stderr, "GLFW Error %d: %s\n", error, description);
    });
    if (!glfwInit())
        return;

    // Decide GL+GLSL versions
#ifdef __EMSCRIPTEN__
    const char* glslVersion = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#else
    const char* glslVersion = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    if (fullscreen)
        glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
#endif

    // Create window with graphics context
    m_pWindow = glfwCreateWindow(defaultWidth, defaultHeight, title, nullptr, nullptr);
    if (!m_pWindow)
        return;

    glfwMakeContextCurrent(m_pWindow);
    glfwSwapInterval(1); // Enable vsync

#ifndef __EMSCRIPTEN__
    glfwSetWindowUserPointer(m_pWindow, this);
    glfwSetFramebufferSizeCallback(m_pWindow, [] (GLFWwindow *window, int, int) {
        Presenter::tick((Presenter*) glfwGetWindowUserPointer(window));
    });
#endif

    if (m_pOnCreate)
        m_pOnCreate(glslVersion, *this);

#ifndef __EMSCRIPTEN__
    while (!glfwWindowShouldClose(m_pWindow))
        tick(this);
#else
    emscripten_set_window_title(title);

    if (fullscreen) {
        EmscriptenFullscreenStrategy strategy;
	    strategy.scaleMode = EMSCRIPTEN_FULLSCREEN_CANVAS_SCALE_STDDEF;
	    strategy.filteringMode = EMSCRIPTEN_FULLSCREEN_FILTERING_DEFAULT;
	    strategy.canvasResizedCallback = emscriptenWindowResizedCallback;
        strategy.canvasResizedCallbackUserData = this;
	    emscripten_enter_soft_fullscreen("canvas", &strategy);
    }

    emscripten_set_main_loop_arg(Presenter::tick, this, 0, true);
#endif
}

void Presenter::tick(void *presenterPtr) {
    const auto &presenter = *((Presenter*) presenterPtr);
    glfwPollEvents();
    if (presenter.m_pOnTick)
        presenter.m_pOnTick(presenter);
    glfwSwapBuffers(presenter.m_pWindow);
}

void Presenter::clear(const GLclampf *backgroundColor) const {
    int w, h;
    glfwGetFramebufferSize(m_pWindow, &w, &h);
    glViewport(0, 0, w, h);
    glClearColor(
        backgroundColor[0] * backgroundColor[3],
        backgroundColor[1] * backgroundColor[3],
        backgroundColor[2] * backgroundColor[3],
        backgroundColor[3]
    );
    glClear(GL_COLOR_BUFFER_BIT);
}
