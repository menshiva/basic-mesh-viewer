#include "meshrenderer/MeshRenderer.hpp"
#include "presenter/Presenter.hpp"
#include "ui/Ui.hpp"
#include "config.hpp"

static bool OnInit(GLFWwindow *Window, MeshRenderer &Renderer, UI &Ui) {
    HelperStructs::GLInfo Info;
    return Renderer.Init(Info) && Ui.Init(Info, Window);
}

static void OnUpdate(MeshRenderer &Renderer, UI &Ui) {
    static ImVec4 MeshColor = *((ImVec4*) Config::MESH_DEFAULT_COLOR);

    Ui.Update(Renderer.GetSelectedMeshIdxRef(), MeshColor);
    Renderer.Update();
}

static void OnDraw(MeshRenderer &Renderer) {
    Renderer.Draw();
    UI::Draw();
}

static void OnDestroy(MeshRenderer &Renderer) {
    Renderer.Destroy();
    UI::Destroy();
}

int main() {
    MeshRenderer Renderer;
    UI Ui;
    return Presenter()
        .WithOnInitCallback([&Renderer, &Ui] (GLFWwindow *Window) {
            return OnInit(Window, Renderer, Ui);
        })
        .WithOnUpdateCallback([&Renderer, &Ui] {
            OnUpdate(Renderer, Ui);
        })
        .WithOnDrawCallback([&Renderer] {
            OnDraw(Renderer);
        })
        .WithOnDestroyCallback([&Renderer] {
            OnDestroy(Renderer);
        })
        .InitGlfwCreateWindowAndLoop();
}
