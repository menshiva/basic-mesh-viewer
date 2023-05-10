#pragma once

#include <functional>
#include <GLFW/glfw3.h>

class Presenter {
public:
    Presenter();
    ~Presenter();

    GLFWwindow *getWindow() const { return m_pWindow; }

    Presenter &withOnCreateCallback(std::function<void(const char*, const Presenter&)> &&Callback);
    Presenter &withOnTickCallback(std::function<void(const Presenter&)> &&Callback);
    Presenter &withOnDestroyCallback(std::function<void(const Presenter&)> &&Callback);

    void initGlfwCreateWindowAndLoop(int defaultWidth, int defaultHeight, const char *title, bool fullscreen);
    void clear(const GLclampf backgroundColor[4]) const;
private:
    static void tick(void *presenterPtr);

    GLFWwindow *m_pWindow;

    std::function<void(const char*, const Presenter&)> m_pOnCreate;
    std::function<void(const Presenter&)> m_pOnTick;
    std::function<void(const Presenter&)> m_pOnDestroy;
};
