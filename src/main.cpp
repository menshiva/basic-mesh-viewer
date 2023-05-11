#include <GL/glew.h>
#include "presenter/Presenter.hpp"
#include "ui/Ui.hpp"
#include "config.hpp"

static bool OnInit(const char *glslVersion, GLFWwindow *window) {
    if (glewInit() != GLEW_OK)
        return false;

    if (!UI::Init(glslVersion, window))
        return false;

    return true;
}

static void OnPreRender() {
    static uint8_t SelectedMeshIdx = 0;
    static ImVec4 MeshColor = *((ImVec4*) Config::MESH_DEFAULT_COLOR);

    UI::PreRender(SelectedMeshIdx, MeshColor);
}

static void OnRender() {
    UI::Render();
}

static void OnDestroy() {
    UI::Destroy();
}

int main() {
    Presenter()
        .WithOnInitCallback(OnInit)
        .WithOnPreRenderCallback(OnPreRender)
        .WithOnRenderCallback(OnRender)
        .WithOnDestroyCallback(OnDestroy)
        .InitGlfwCreateWindowAndLoop();
    return 0;
}
