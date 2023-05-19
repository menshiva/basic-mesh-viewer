#define GLM_FORCE_MESSAGES
#include "meshrenderer/MeshRenderer.hpp"
#include "ui/Ui.hpp"
#include "presenter/Presenter.hpp"

static bool OnInit(MeshRenderer *renderer, UI *ui, GLFWwindow *window) {
    GLInfo glInfo;
    return renderer->Init(glInfo) && ui->Init(glInfo, window);
}

static void OnResize(MeshRenderer *renderer, UI*, const int w, const int h) {
    renderer->Resize(w, h);
}

static void OnUpdate(MeshRenderer *renderer, UI *ui) {
    ui->Update(
        renderer->GetSelectedMeshIdxRef(),
        renderer->GetMeshesNames(),
        renderer->GetIsColorSpecifiedRef(),
        renderer->GetSpecifiedColorRef()
    );
    renderer->Update(
        ui->m_DeltaTime,
        ui->IsSelectedMeshIdxChanged(),
        ui->IsColorSpecifiedChanged(),
        ui->IsColorChanged()
    );
}

static void OnDraw(MeshRenderer *renderer, UI*) {
    renderer->Draw();
    UI::Draw();
}

static void OnDestroy(MeshRenderer *renderer, UI*) {
    renderer->Destroy();
    UI::Destroy();
}

int main() {
    MeshRenderer renderer;
    UI ui;
    return Presenter<MeshRenderer*, UI*>(&renderer, &ui)
        .WithOnInitCallback(OnInit)
        .WithOnResizeCallback(OnResize)
        .WithOnUpdateCallback(OnUpdate)
        .WithOnDrawCallback(OnDraw)
        .WithOnDestroyCallback(OnDestroy)
        .InitGlfwCreateWindowAndLoop();
}
