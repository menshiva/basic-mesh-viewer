#pragma once

#include <cstdint>
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "../helper.hpp"

class UI {
public:
    bool Init(const HelperStructs::GLInfo &Info, struct GLFWwindow *Window);
    void Update(uint8_t &SelectedMeshIdx, ImVec4 &MeshColor) const;
    static void Draw();
    static void Destroy();
private:
    static void MeshSection(uint8_t &SelectedMeshIdx);
    static void ColorSection(float AvailableParentWidth, ImVec4 &MeshColor);
    static void InfoAndMetricsSection(const HelperStructs::GLInfo &Info, float Framerate);

    HelperStructs::GLInfo m_pInfo;
};
