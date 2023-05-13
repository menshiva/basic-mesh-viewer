#pragma once

#include <functional>
#include <GLFW/glfw3.h>

class Presenter {
public:
    Presenter();
    ~Presenter();

    GLFWwindow *GetWindow() const { return m_pWindow; }

    Presenter &WithOnInitCallback(std::function<bool(GLFWwindow*)> &&Callback);
    Presenter &WithOnUpdateCallback(std::function<void()> &&Callback);
    Presenter &WithOnDrawCallback(std::function<void()> &&Callback);
    Presenter &WithOnDestroyCallback(std::function<void()> &&Callback);

    int InitGlfwCreateWindowAndLoop();
private:
    static GLFWwindow *InitWindow(Presenter *UserPointer);
    static void Tick(void *PresenterPtr);

    GLFWwindow *m_pWindow;

    std::function<bool(GLFWwindow*)> m_pOnInit;
    std::function<void()> m_pOnUpdate, m_pOnDraw;
    std::function<void()> m_pOnDestroy;
};
