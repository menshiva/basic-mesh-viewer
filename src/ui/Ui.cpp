#include "Ui.hpp"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"
#include "../meshrenderer/mesh/MeshSettings.hpp"

// TODO: make adaptive
// TODO: better touch gestures on smartphones

bool UI::Init(const GLInfo &glInfo, GLFWwindow *window) {
    m_pInfo = glInfo;

    IMGUI_CHECKVERSION();

    const auto ctx = ImGui::CreateContext();
    if (!ctx || !ctx->Initialized)
        return false;

    ImGui::StyleColorsDark();
    ImGuiStyle &style = ImGui::GetStyle();
    style.WindowBorderSize = 0.0f;
    style.WindowRounding = 4.0f;
    style.FrameRounding = 4.0f;
    style.ScrollbarRounding = 4.0f;
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

void UI::Update(
    const std::vector<std::string_view> &meshesNames,
    UpdateParams &updateParams,
    MeshSettings *meshSettings,
    DrawInfo drawInfo
) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    const auto &viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(
        ImVec2(viewport->Pos.x + viewport->Size.x, 0.0f),
        ImGuiCond_Always, ImVec2(1.0f, 0.0f)
    );

    constexpr static float dummyHeight = 2.0f;
    float windowWidth = 0.0f;
    ImGui::Begin("##Main##", nullptr);

    ImGui::SeparatorText("Mesh");
    windowWidth = ImMax(TypeSection(meshesNames, updateParams.m_SelectedMeshIdx), windowWidth);
    ImGui::Dummy(ImVec2(0.0f, dummyHeight));
    windowWidth = ImMax(ColorSection(updateParams.m_ColorType, meshSettings->m_Color, updateParams.m_OverrideColor), windowWidth);
    ImGui::Dummy(ImVec2(0.0f, dummyHeight));
    ScaleSection(updateParams.m_Scale);
    // TODO
    // ImGui::Dummy(ImVec2(0.0f, dummyHeight));
    // ImGui::Text("Use LMB to rotate.");

    if (auto poly2DSettings = dynamic_cast<Polygon2DSettings*>(meshSettings)) {
        ImGui::SeparatorText("Preferences");
        VerticesNumSection(poly2DSettings);
    }

    ImGui::SeparatorText("Info and Metrics");
    InfoAndMetricsSection(drawInfo);

    ImGui::SetWindowSize(ImVec2(windowWidth, 0.0f));
    ImGui::End();

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

float UI::TypeSection(const std::vector<std::string_view> &meshesNames, int &selectedMeshIdx) {
    ImGui::PushItemWidth(-FLT_MIN);

    const char *previewStr, *previewStrEnd;
    ImFormatStringToTempBuffer(&previewStr, &previewStrEnd, "Type: %s", meshesNames[selectedMeshIdx].data());

    if (ImGui::BeginCombo("##Type##", previewStr)) {
        for (int i = 0; i < meshesNames.size(); ++i) {
            const bool isSelected = (i == selectedMeshIdx);
            if (ImGui::Selectable(meshesNames[i].data(), isSelected))
                selectedMeshIdx = i;
            if (isSelected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    ImGui::PopItemWidth();

    const static float typeSize = ImGui::CalcTextSize("Type: ").x;
    float maxBodySize = 0.0f;
    for (const auto s : meshesNames) {
        const float size = ImGui::CalcTextSize(s.data(), s.data() + s.size()).x;
        if (size > maxBodySize)
            maxBodySize = size;
    }
    constexpr static float dropButtonSize = 40.0f;

    return typeSize + maxBodySize + dropButtonSize + 2.0f;
}

float UI::ColorSection(UpdateParams::ColorType &colorType, glm::vec3 &specifiedColor, glm::vec3 &overrideColor) {
    const static auto ToStr = [] (UpdateParams::ColorType v) {
        switch (v) {
            case UpdateParams::ColorType::INTERPOLATE:      return "Interpolate";
            case UpdateParams::ColorType::HUE_DISTRIBUTE:   return "Hue uniform distribution";
            case UpdateParams::ColorType::SPECIFIED:        return "Specified";
            default:                                        return "[Unknown ColorType]";
        }
    };
    constexpr static float colorEdit4WidthPadding = 43.0f;
    static auto prevColorType = UpdateParams::ColorType::NUM;

    if (prevColorType != UpdateParams::ColorType::SPECIFIED)
        ImGui::PushItemWidth(-FLT_MIN);
    else
        ImGui::PushItemWidth(ImGui::GetWindowWidth() - colorEdit4WidthPadding);

    const char *previewStr, *previewStrEnd;
    ImFormatStringToTempBuffer(&previewStr, &previewStrEnd, "Color: %s", ToStr(colorType));

    if (ImGui::BeginCombo("##Color Type##", previewStr)) {
        std::underlying_type_t<UpdateParams::ColorType> i;
        auto num = (std::underlying_type_t<UpdateParams::ColorType>) UpdateParams::ColorType::NUM;

        for (i = 0; i < num; ++i) {
            const bool isSelected = (i == (std::underlying_type_t<UpdateParams::ColorType>) colorType);
            if (ImGui::Selectable(ToStr((UpdateParams::ColorType) i), isSelected))
                colorType = (UpdateParams::ColorType) i;
            if (isSelected)
                ImGui::SetItemDefaultFocus();
        }

        ImGui::EndCombo();
    }

    if (colorType == UpdateParams::ColorType::SPECIFIED) {
        ImGui::SameLine();
        ImGui::ColorEdit4(
            "##Mesh Color##", (float*) &specifiedColor,
            ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_InputRGB
            | ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_DisplayRGB
        );
    }

    ImGui::PopItemWidth();

    if (colorType == UpdateParams::ColorType::SPECIFIED) {
        overrideColor = specifiedColor;
    }
    else if (colorType == UpdateParams::ColorType::INTERPOLATE) {
        if (prevColorType == colorType) {
            glm::vec3 hsv;
            ImGui::ColorConvertRGBtoHSV(
                overrideColor.r, overrideColor.g, overrideColor.b,
                hsv.x, hsv.y, hsv.z
            );
            hsv.x += ImGui::GetIO().DeltaTime * 0.5f;
            if (hsv.x > 360.0f)
                hsv.x -= 360.0f;
            ImGui::ColorConvertHSVtoRGB(
                hsv.x, hsv.y, hsv.z,
                overrideColor.r, overrideColor.g, overrideColor.b
            );
        }
        else
            overrideColor = glm::vec3(1.0f, 0.0f, 0.0f);
    }

    prevColorType = colorType;

    const static float colorSize = ImGui::CalcTextSize("Color: ").x;
    float maxBodySize = 0.0f;
    std::underlying_type_t<UpdateParams::ColorType> i;
    auto num = (std::underlying_type_t<UpdateParams::ColorType>) UpdateParams::ColorType::NUM;
    for (i = 0; i < num; ++i) {
        const float size = ImGui::CalcTextSize(ToStr((UpdateParams::ColorType) i)).x;
        if (size > maxBodySize)
            maxBodySize = size;
    }
    constexpr static float dropButtonSize = 40.0f;

    return colorSize + maxBodySize + dropButtonSize + 2.0f;
}

void UI::ScaleSection(float &scale) {
    ImGui::PushItemWidth(-FLT_MIN);
    ImGui::SliderFloat(
        "##Scale##", &scale,
        Config::MESH_SETTINGS_SCALE_MIN, Config::MESH_SETTINGS_SCALE_MAX,
        "Scale: %.2f (or use mouse wheel)",
        ImGuiSliderFlags_NoInput
    );
    ImGui::PopItemWidth();

    const auto &io = ImGui::GetIO();
    if (!io.WantCaptureMouse) {
        scale += io.MouseWheel * 0.05f;
        scale = ImClamp(scale, Config::MESH_SETTINGS_SCALE_MIN, Config::MESH_SETTINGS_SCALE_MAX);
    }
}

void UI::VerticesNumSection(Polygon2DSettings *poly2DSettings) {
    ImGui::PushItemWidth(-FLT_MIN);
    ImGui::SliderInt(
        "##Vertices##", &poly2DSettings->m_VerticesNum,
        Config::MESH_SETTINGS_POLY2D_VERTICES_MIN, Config::MESH_SETTINGS_POLY2D_VERTICES_MAX,
        "Vertices: %d",
        ImGuiSliderFlags_NoInput
    );
    ImGui::PopItemWidth();
}

void UI::InfoAndMetricsSection(DrawInfo drawInfo) const {
    ImGui::Text("Vertices: %d", drawInfo.m_VerticesNum);
    ImGui::Text("Triangles: %d", drawInfo.m_TrianglesNum);
    ImGui::Text("Performance: %.1f FPS", ImGui::GetIO().Framerate);

    ImGui::Separator();

    ImGui::TextWrapped("OpenGL implementation vendor: %s", m_pInfo.m_Vendor.data());
    ImGui::TextWrapped("Renderer: %s", m_pInfo.m_Renderer.data());
    ImGui::TextWrapped("OpenGL version supported: %s", m_pInfo.m_Version.data());
    ImGui::TextWrapped("OpenGL shading language: %s", m_pInfo.m_ShadingLanguageVersion.data());
}
