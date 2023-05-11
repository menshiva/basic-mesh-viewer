#include "config.hpp"
#include "presenter/Presenter.hpp"
#include "ui/Ui.hpp"

static void OnInit(const char *glslVersion, GLFWwindow *window) {
    UI::Init(glslVersion, window);
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
