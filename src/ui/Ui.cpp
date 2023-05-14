#include "Ui.hpp"
#include <cstdio>
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"

// TODO: make adaptive
// TODO: better touch gestures on smartphones

bool UI::Init(const HelperStructs::GLInfo &glInfo, GLFWwindow *window) {
    m_pInfo = glInfo;

    IMGUI_CHECKVERSION();

    const auto ctx = ImGui::CreateContext();
    if (!ctx || !ctx->Initialized)
        return false;

    ImGui::StyleColorsDark();
    ImGuiStyle &style = ImGui::GetStyle();
    // TODO
    // style.WindowBorderSize = 0.0f;
    // style.PopupBorderSize = 0.0f;
    style.WindowRounding = 4.0f;
    style.FrameRounding = 4.0f;
    // style.PopupRounding = 4.0f;
    style.ScrollbarRounding = 4.0f;
    // style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.65);
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.0f, 0.0f, 0.0f, 0.65);

    if (!ImGui_ImplGlfw_InitForOpenGL(window, true))
        return false;

    if (!ImGui_ImplOpenGL3_Init(GLSL_VERSION))
        return false;

#ifdef __EMSCRIPTEN__
    ImGui::GetIO().IniFilename = nullptr;
#endif

    return true;
}

void UI::Update(uint8_t &selectedMeshIdx, ImVec4 &meshColor) const {
    auto &io = ImGui::GetIO();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // TODO
    // hard-coded for now
    // ImGui::SetNextWindowSize(ImVec2(338.0f, 475.0f), ImGuiCond_Once);

    // const auto &viewport = ImGui::GetMainViewport();
    // ImGui::SetNextWindowPos(
    //     ImVec2(viewport->Pos.x + viewport->Size.x, 0.0f),
    //     ImGuiCond_Always, ImVec2(1.0f, 0.0f)
    // );

    ImGui::Begin("##Main##", nullptr/*, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove*/);
    const float availableWidth = ImGui::GetContentRegionAvail().x;

    MeshSection(selectedMeshIdx);
    ColorSection(availableWidth, meshColor);
    InfoAndMetricsSection(m_pInfo, io.Framerate);

    ImGui::End();

    // TODO
    /*static bool showDemoWindow = true;
    static bool showAnotherWindow = false;

    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
    if (showDemoWindow)
        ImGui::ShowDemoWindow(&showDemoWindow);

    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
    {
        static float f = 0.0f;
        static int counter = 0;
        static auto editColor = meshColor;

        ImGui::Begin("Hello, world!"); // Create a window called "Hello, world!" and append into it.

        ImGui::Text("This is some useful text."); // Display some text (you can use a format strings too)
        ImGui::Checkbox("Demo Window", &showDemoWindow); // Edit bools storing our window open/close state
        ImGui::Checkbox("Another Window", &showAnotherWindow);

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
    if (showAnotherWindow) {
        ImGui::Begin("Another Window", &showAnotherWindow); // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        ImGui::Text("Hello from another window!");
        if (ImGui::Button("Close Me"))
            showAnotherWindow = false;
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

void UI::MeshSection(uint8_t &selectedMeshIdx) {
    ImGui::SeparatorText("Mesh");
    for (uint8_t i = 0; i < 5; ++i) {
        char buf[32];
        sprintf(buf, "Object %d", i);
        if (ImGui::Selectable(buf, selectedMeshIdx == i))
            selectedMeshIdx = i;
    }
}

void UI::ColorSection(const float availableParentWidth, ImVec4 &meshColor) {
    ImGui::SeparatorText("Color");
    const float colorOffset = (availableParentWidth - ImGui::CalcItemWidth()) * 0.5f;
    if (colorOffset > 0.0f)
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + colorOffset);
    // TODO: color options
    ImGui::ColorPicker3(
        "##Mesh Color##", (float*) &meshColor,
        ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoAlpha
    );
}

void UI::InfoAndMetricsSection(const HelperStructs::GLInfo &glInfo, const float framerate) {
    ImGui::SeparatorText("Info and Metrics");

    ImGui::Text("OpenGL implementation vendor: %s", glInfo.m_Vendor.data());
    ImGui::Text("Renderer: %s", glInfo.m_Renderer.data());
    ImGui::Text("OpenGL version supported: %s", glInfo.m_Version.data());
    ImGui::Text("OpenGL shading language: %s", glInfo.m_ShadingLanguageVersion.data());

    ImGui::Separator();

    // TODO
    // const float metricsOffset = (availableWidth - ImGui::CalcTextSize()) * 0.5f;
    // if (metricsOffset > 0.0f)
    //     ImGui::SetCursorPosX(ImGui::GetCursorPosX() + metricsOffset);
    ImGui::Text("Performance: %.1f FPS", framerate);
}
