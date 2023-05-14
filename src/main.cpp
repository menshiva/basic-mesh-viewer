#include "meshrenderer/MeshRenderer.hpp"
#include "presenter/Presenter.hpp"
#include "ui/Ui.hpp"
#include "config.hpp"

static bool OnInit(GLFWwindow *window, MeshRenderer &renderer, UI &ui) {
    HelperStructs::GLInfo glInfo;
    return renderer.Init(glInfo) && ui.Init(glInfo, window);
}

static void OnUpdate(MeshRenderer &renderer, UI &ui) {
    static ImVec4 meshColor = *((ImVec4*) Config::MESH_DEFAULT_COLOR);

    ui.Update(renderer.GetSelectedMeshIdxRef(), meshColor);
    renderer.Update();
}

static void OnDraw(MeshRenderer &renderer) {
    renderer.Draw();
    UI::Draw();
}

static void OnDestroy(MeshRenderer &renderer) {
    renderer.Destroy();
    UI::Destroy();
}

int main() {
    MeshRenderer renderer;
    UI ui;
    return Presenter()
        .WithOnInitCallback([&renderer, &ui] (GLFWwindow *window) {
            return OnInit(window, renderer, ui);
        })
        .WithOnUpdateCallback([&renderer, &ui] {
            OnUpdate(renderer, ui);
        })
        .WithOnDrawCallback([&renderer] {
            OnDraw(renderer);
        })
        .WithOnDestroyCallback([&renderer] {
            OnDestroy(renderer);
        })
        .InitGlfwCreateWindowAndLoop();
}
