#pragma once

#include <functional>
#include <cstdio>
#include <GLFW/glfw3.h>
#include "../config.hpp"
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#endif

template <typename... UserData>
class Presenter {
public:
    explicit Presenter(UserData&&... args) : m_pUserData(std::forward<UserData>(args)...), m_pWindow(nullptr) {}

    ~Presenter() {
        std::apply(m_pOnDestroy, m_pUserData);
        glfwDestroyWindow(m_pWindow);
        glfwTerminate();
    }

    Presenter &WithOnInitCallback(auto &&callback) { m_pOnInit = callback; return *this; }
    Presenter &WithOnResizeCallback(auto &&callback) { m_pOnResize = callback; return *this; }
    Presenter &WithOnUpdateCallback(auto &&callback) { m_pOnUpdate = callback; return *this; }
    Presenter &WithOnDrawCallback(auto &&callback) { m_pOnDraw = callback; return *this; }
    Presenter &WithOnDestroyCallback(auto &&callback) { m_pOnDestroy = callback; return *this; }

    int InitGlfwCreateWindowAndLoop() {
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

        if (!std::apply(m_pOnInit, std::tuple_cat(m_pUserData, std::make_tuple(m_pWindow))))
            return 1;

    #ifndef __EMSCRIPTEN__
        int w, h;
        glfwGetFramebufferSize(m_pWindow, &w, &h);
        OnWindowResized(w, h);
        while (!glfwWindowShouldClose(m_pWindow))
            Tick();
    #else
        emscripten_set_window_title(Config::WIN_TITLE);
        OnWindowResizedEmscripten(EMSCRIPTEN_EVENT_RESIZE, nullptr, this);
        emscripten_set_main_loop_arg([] (void *userData) { ((Presenter*) userData)->Tick(); }, this, 0, true);
    #endif

        return 0;
    }
private:
    static GLFWwindow *InitWindow(void *presenterPtr) {
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

        glfwSetWindowUserPointer(window, presenterPtr);
        glfwSetFramebufferSizeCallback(window, [] (GLFWwindow *window, int w, int h) {
            ((Presenter*) glfwGetWindowUserPointer(window))->OnWindowResized(w, h);
        });
    #else
        emscripten_set_resize_callback(
            EMSCRIPTEN_EVENT_TARGET_WINDOW, presenterPtr, EM_TRUE,
            ([] (const int eventType, const EmscriptenUiEvent *uiEvent, void *userData) {
                return ((Presenter*) userData)->OnWindowResizedEmscripten(eventType, uiEvent, userData);
            })
        );
    #endif

        return window;
    }

#ifdef __EMSCRIPTEN__
    EM_BOOL OnWindowResizedEmscripten(int eventType, const struct EmscriptenUiEvent*, void *userData) const {
        if (eventType == EMSCRIPTEN_EVENT_RESIZE) {
            double width, height;
            emscripten_get_element_css_size("canvas", &width, &height);

            auto &presenter = *((Presenter*) userData);
            glfwSetWindowSize(presenter.m_pWindow, width, height);
            presenter.OnWindowResized(width, height);

            return true;
        }
        return false;
    }
#endif

    void OnWindowResized(int w, int h) const {
        glViewport(0, 0, w, h);
        std::apply(m_pOnResize, std::tuple_cat(m_pUserData, std::make_tuple(w, h)));
    #ifndef __EMSCRIPTEN__
        Tick();
    #endif
    }

    void Tick() const {
        glfwPollEvents();

        std::apply(m_pOnUpdate, m_pUserData);

        glClearColor(Config::BACKGROUND_COLOR.r, Config::BACKGROUND_COLOR.g, Config::BACKGROUND_COLOR.b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        std::apply(m_pOnDraw, m_pUserData);

        glfwSwapBuffers(m_pWindow);
    }

    std::tuple<UserData...> m_pUserData;
    GLFWwindow *m_pWindow;

    std::function<bool(UserData..., GLFWwindow *window)> m_pOnInit;
    std::function<void(UserData..., int w, int h)> m_pOnResize;
    std::function<void(UserData...)> m_pOnUpdate, m_pOnDraw, m_pOnDestroy;
};
