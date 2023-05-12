#include "meshrenderer/MeshRenderer.hpp"
#include "presenter/Presenter.hpp"
#include "ui/Ui.hpp"
#include "config.hpp"

static bool OnInit(MeshRenderer &Renderer, const char *glslVersion, GLFWwindow *window) {
    return Renderer.Init() && UI::Init(glslVersion, window);
}

static void OnPreRender(MeshRenderer &Renderer) {
    static ImVec4 MeshColor = *((ImVec4*) Config::MESH_DEFAULT_COLOR);

    UI::PreRender(Renderer.GetSelectedMeshIdxRef(), MeshColor);
    Renderer.PreRender();
}

static void OnRender(MeshRenderer &Renderer) {
    Renderer.Render();
    UI::Render();
}

static void OnDestroy(MeshRenderer &Renderer) {
    Renderer.Destroy();
    UI::Destroy();
}

int main() {
    MeshRenderer Renderer;
    Presenter()
        .WithOnInitCallback([&Renderer] (const char *glslVersion, GLFWwindow *window) {
            return OnInit(Renderer, glslVersion, window);
        })
        .WithOnPreRenderCallback([&Renderer] {
            OnPreRender(Renderer);
        })
        .WithOnRenderCallback([&Renderer] {
            OnRender(Renderer);
        })
        .WithOnDestroyCallback([&Renderer] {
            OnDestroy(Renderer);
        })
        .InitGlfwCreateWindowAndLoop();
    return 0;
}
