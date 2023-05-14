#pragma once

#include <functional>
#include <GLFW/glfw3.h>

class Presenter {
public:
    Presenter();
    ~Presenter();

    Presenter &WithOnInitCallback(std::function<bool(GLFWwindow*)> &&callback);
    Presenter &WithOnUpdateCallback(std::function<void()> &&callback);
    Presenter &WithOnDrawCallback(std::function<void()> &&callback);
    Presenter &WithOnDestroyCallback(std::function<void()> &&callback);

    int InitGlfwCreateWindowAndLoop();

    GLFWwindow *GetWindow() const { return m_pWindow; }
private:
    static GLFWwindow *InitWindow(Presenter *userPointer);
    static void Tick(void *presenterPtr);

    GLFWwindow *m_pWindow;

    std::function<bool(GLFWwindow*)> m_pOnInit;
    std::function<void()> m_pOnUpdate, m_pOnDraw;
    std::function<void()> m_pOnDestroy;
};
