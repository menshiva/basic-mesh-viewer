#pragma once

#include <functional>

class Presenter {
public:
    Presenter();
    ~Presenter();

    Presenter &WithOnInitCallback(std::function<bool(struct GLFWwindow*)> &&callback);
    Presenter &WithOnUpdateCallback(std::function<void()> &&callback);
    Presenter &WithOnDrawCallback(std::function<void()> &&callback);
    Presenter &WithOnDestroyCallback(std::function<void()> &&callback);

    int InitGlfwCreateWindowAndLoop();
private:
    static GLFWwindow *InitWindow(Presenter *userPointer);

#ifdef __EMSCRIPTEN__
    static int EmscriptenWindowResizedCallback(int eventType, const struct EmscriptenUiEvent*, void *userData);
#endif
    static void OnWindowResized(GLFWwindow *window, int w, int h);

    static void Tick(void *presenterPtr);

    GLFWwindow *m_pWindow;

    std::function<bool(GLFWwindow*)> m_pOnInit;
    std::function<void()> m_pOnUpdate, m_pOnDraw;
    std::function<void()> m_pOnDestroy;
};
