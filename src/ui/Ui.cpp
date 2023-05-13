#include "Ui.hpp"
#include <cstdio>
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"

bool UI::Init(const HelperStructs::GLInfo &Info, GLFWwindow *Window) {
    m_pInfo = Info;

    IMGUI_CHECKVERSION();

    const auto Ctx = ImGui::CreateContext();
    if (!Ctx || !Ctx->Initialized)
        return false;

    ImGui::StyleColorsDark();

    if (!ImGui_ImplGlfw_InitForOpenGL(Window, true))
        return false;

    if (!ImGui_ImplOpenGL3_Init(GLSL_VERSION))
        return false;

#ifdef __EMSCRIPTEN__
    // For an Emscripten build we are disabling file-system access, so let's not attempt to do a fopen() of the imgui.ini file.
    ImGui::GetIO().IniFilename = nullptr;
#endif

    return true;
}

void UI::Update(uint8_t &SelectedMeshIdx, ImVec4 &MeshColor) const {
    // TODO: make adaptive

    auto &IO = ImGui::GetIO();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // hard-coded for now
    ImGui::SetNextWindowSize(ImVec2(338.0f, 475.0f), ImGuiCond_Once);

    const auto &Viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(
        ImVec2(Viewport->Pos.x + Viewport->Size.x, 0.0f),
        ImGuiCond_Always, ImVec2(1.0f, 0.0f)
    );

    ImGui::Begin("##Main##", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
    const float AvailableWidth = ImGui::GetContentRegionAvail().x;

    MeshSection(SelectedMeshIdx);
    ColorSection(AvailableWidth, MeshColor);
    InfoAndMetricsSection(m_pInfo, IO.Framerate);

    ImGui::End();

    // TODO
    /*static bool ShowDemoWindow = true;
    static bool ShowAnotherWindow = false;

    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
    if (ShowDemoWindow)
        ImGui::ShowDemoWindow(&ShowDemoWindow);

    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
    {
        static float f = 0.0f;
        static int counter = 0;
        static auto editColor = MeshColor;

        ImGui::Begin("Hello, world!"); // Create a window called "Hello, world!" and append into it.

        ImGui::Text("This is some useful text."); // Display some text (you can use a format strings too)
        ImGui::Checkbox("Demo Window", &ShowDemoWindow); // Edit bools storing our window open/close state
        ImGui::Checkbox("Another Window", &ShowAnotherWindow);

        ImGui::SliderFloat("float", &f, 0.0f, 1.0f); // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::ColorEdit3("clear color", (float*) &editColor); // Edit 3 floats representing a color

        if (ImGui::Button("Button")) // Buttons return true when clicked (most widgets return true when edited/activated)
            ++counter;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::End();
    }

    // 3. Show another simple window.
    if (ShowAnotherWindow) {
        ImGui::Begin("Another Window", &ShowAnotherWindow); // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        ImGui::Text("Hello from another window!");
        if (ImGui::Button("Close Me"))
            ShowAnotherWindow = false;
        ImGui::End();
    }*/

    ImGui::EndFrame();
}

void UI::Draw() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UI::Destroy() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void UI::MeshSection(uint8_t &SelectedMeshIdx) {
    ImGui::SeparatorText("Mesh");
    for (uint8_t i = 0; i < 5; ++i) {
        char buf[32];
        sprintf(buf, "Object %d", i);
        if (ImGui::Selectable(buf, SelectedMeshIdx == i))
            SelectedMeshIdx = i;
    }
}

void UI::ColorSection(const float AvailableParentWidth, ImVec4 &MeshColor) {
    ImGui::SeparatorText("Color");
    const float colorOffset = (AvailableParentWidth - ImGui::CalcItemWidth()) * 0.5f;
    if (colorOffset > 0.0f)
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + colorOffset);
    ImGui::ColorPicker3(
        "##Mesh Color##", (float*) &MeshColor,
        ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoAlpha
    );
}

void UI::InfoAndMetricsSection(const HelperStructs::GLInfo &Info, const float Framerate) {
    ImGui::SeparatorText("Info and Metrics");

    ImGui::Text("OpenGL implementation vendor: %s", Info.m_Vendor.data());
    ImGui::Text("Renderer: %s", Info.m_Renderer.data());
    ImGui::Text("OpenGL version supported: %s", Info.m_Version.data());
    ImGui::Text("OpenGL shading language: %s", Info.m_ShadingLanguageVersion.data());

    ImGui::Separator();

    // TODO
    // const float metricsOffset = (availableWidth - ImGui::CalcTextSize()) * 0.5f;
    // if (metricsOffset > 0.0f)
    //     ImGui::SetCursorPosX(ImGui::GetCursorPosX() + metricsOffset);
    ImGui::Text("Performance: %.1f FPS", Framerate);
}
