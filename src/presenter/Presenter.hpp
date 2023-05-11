#pragma once

#include <functional>
#include <GLFW/glfw3.h>

class Presenter {
public:
    Presenter();
    ~Presenter();

    GLFWwindow *GetWindow() const { return m_pWindow; }

    Presenter &WithOnInitCallback(std::function<bool(const char*, GLFWwindow*)> &&Callback);
    Presenter &WithOnPreRenderCallback(std::function<void()> &&Callback);
    Presenter &WithOnRenderCallback(std::function<void()> &&Callback);
    Presenter &WithOnDestroyCallback(std::function<void()> &&Callback);

    void InitGlfwCreateWindowAndLoop();
private:
    static void Tick(void *PresenterPtr);

    GLFWwindow *m_pWindow;

    std::function<bool(const char*, GLFWwindow*)> m_pOnInit;
    std::function<void()> m_pOnPreRender, m_pOnRender, m_pOnDestroy;
};
