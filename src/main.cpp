#include "meshrenderer/MeshRenderer.hpp"
#include "presenter/Presenter.hpp"
#include "ui/Ui.hpp"

static bool OnInit(GLFWwindow *window, MeshRenderer &renderer, UI &ui) {
    GLInfo glInfo;
    return renderer.Init(glInfo) && ui.Init(glInfo, window);
}

static void OnUpdate(MeshRenderer &renderer, UI &ui) {
    ui.Update(renderer.GetSelectedMeshIdxRef(), renderer.GetMeshColorRef());
    MeshRenderer::Update();
}

static void OnDraw() {
    MeshRenderer::Draw();
    UI::Draw();
}

static void OnDestroy(MeshRenderer &renderer) {
    renderer.Destroy();
    UI::Destroy();
}

int main() {
    MeshRenderer renderer;

    UI ui;
    ui
        .WithOnSelectedMeshIdxChangedCallback([&renderer] {
            renderer.OnSelectedMeshIdxChanged();
        })
        .WithOnColorChangedCallback([&renderer] {
            renderer.OnMeshColorChanged();
        });

    Presenter presenter;
    presenter
        .WithOnInitCallback([&renderer, &ui] (GLFWwindow *window) {
            return OnInit(window, renderer, ui);
        })
        .WithOnUpdateCallback([&renderer, &ui] {
            OnUpdate(renderer, ui);
        })
        .WithOnDrawCallback(OnDraw)
        .WithOnDestroyCallback([&renderer] {
            OnDestroy(renderer);
        });

    return presenter.InitGlfwCreateWindowAndLoop();
}
