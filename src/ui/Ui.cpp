#include "Ui.hpp"
#include <vector>
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"
#include "../config.hpp"

// TODO: make adaptive
// TODO: better touch gestures on smartphones

UI::UI() : m_DeltaTime(0.0f), m_pIsSelectedMeshIdxChanged(false), m_pIsColorSpecifiedChanged(false), m_pColorChanged(false) {}

bool UI::Init(const GLInfo &glInfo, GLFWwindow *window) {
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

void UI::Update(int &selectedMeshIdx, const std::vector<const char*> &meshNames, bool &isColorSpecified, float *color) {
    auto &io = ImGui::GetIO();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // TODO
    // hard-coded for now
    ImGui::SetNextWindowSize(ImVec2(389.0f, 236.0f), ImGuiCond_Once);

    const auto &viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(
        ImVec2(viewport->Pos.x + viewport->Size.x, 0.0f),
        ImGuiCond_Always, ImVec2(1.0f, 0.0f)
    );

    ImGui::Begin("##Main##", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
    TypeSection(selectedMeshIdx, meshNames);
    SettingsSection();
    ColorSection(isColorSpecified, color);
    InfoAndMetricsSection();
    ImGui::End();

    m_DeltaTime = io.DeltaTime;
    ImGui::EndFrame();
}

void UI::Draw() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UI::Destroy() {
    const auto Ctx = ImGui::GetCurrentContext();
    if (Ctx && Ctx->Initialized) {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }
}

void UI::TypeSection(int &selectedMeshIdx, const std::vector<const char*> &meshNames) {
    ImGui::SeparatorText("Type");
    m_pIsSelectedMeshIdxChanged = ImGui::Combo("##Type##", &selectedMeshIdx, meshNames.data(), (int) meshNames.size());
}

void UI::SettingsSection() {

}

void UI::ColorSection(bool &isSpecified, float *color) {
    ImGui::SeparatorText("Color");
    m_pIsColorSpecifiedChanged = ImGui::Checkbox("##Specified##", &isSpecified);
    ImGui::SameLine();
    ImGui::BeginDisabled(!isSpecified);
    m_pColorChanged = ImGui::ColorEdit3(
        "##Mesh Color##", color,
        ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_DisplayRGB
    );
    ImGui::EndDisabled();
}

void UI::InfoAndMetricsSection() const {
    ImGui::SeparatorText("Info and Metrics");

    ImGui::Text("OpenGL implementation vendor: %s", m_pInfo.m_Vendor.data());
    ImGui::Text("Renderer: %s", m_pInfo.m_Renderer.data());
    ImGui::Text("OpenGL version supported: %s", m_pInfo.m_Version.data());
    ImGui::Text("OpenGL shading language: %s", m_pInfo.m_ShadingLanguageVersion.data());

    ImGui::Separator();

    // TODO
    // const float metricsOffset = (availableWidth - ImGui::CalcTextSize()) * 0.5f;
    // if (metricsOffset > 0.0f)
    //     ImGui::SetCursorPosX(ImGui::GetCursorPosX() + metricsOffset);
    ImGui::Text("Performance: %.1f FPS", ImGui::GetIO().Framerate);
}

bool UI::GetPropertyAndReset(bool &property) {
    const bool ret = property;
    property = false;
    return ret;
}
