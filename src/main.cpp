#define GLM_FORCE_MESSAGES
#include "meshrenderer/MeshRenderer.hpp"
#include "ui/Ui.hpp"
#include "presenter/Presenter.hpp"

static bool OnInit(MeshRenderer *renderer, UI *ui, GLFWwindow *window) {
    GLInfo glInfo;
    return renderer->Init(glInfo) && ui->Init(glInfo, window);
}

static void OnResize(MeshRenderer *renderer, UI*, const int w, const int h) {
    // TODO
}

static void OnUpdate(MeshRenderer *renderer, UI *ui) {
    ui->Update(renderer->GetIsColorSpecifiedRef(), renderer->GetSpecifiedColorRef());
    renderer->Update(ui->m_DeltaTime);
}

static void OnDraw(MeshRenderer*, UI*) {
    MeshRenderer::Draw();
    UI::Draw();
}

static void OnDestroy(MeshRenderer *renderer, UI*) {
    renderer->Destroy();
    UI::Destroy();
}

int main() {
    MeshRenderer renderer;

    UI ui;
    ui
        .WithOnIsColorSpecifiedChangedCallback([&renderer] {
            renderer.OnIsColorSpecifiedChanged();
        })
        .WithOnSpecifiedColorChangedCallback([&renderer] {
            renderer.OnSpecifiedColorChanged();
        });

    return Presenter<MeshRenderer*, UI*>(&renderer, &ui)
        .WithOnInitCallback(OnInit)
        .WithOnResizeCallback(OnResize)
        .WithOnUpdateCallback(OnUpdate)
        .WithOnDrawCallback(OnDraw)
        .WithOnDestroyCallback(OnDestroy)
        .InitGlfwCreateWindowAndLoop();
}
