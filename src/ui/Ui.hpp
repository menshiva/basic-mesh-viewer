#pragma once

#include <cstdint>
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "../helper.hpp"

class UI {
public:
    bool Init(const HelperStructs::GLInfo &glInfo, struct GLFWwindow *window);
    void Update(uint8_t &selectedMeshIdx, ImVec4 &meshColor) const;
    static void Draw();
    static void Destroy();
private:
    static void MeshSection(uint8_t &selectedMeshIdx);
    static void ColorSection(float availableParentWidth, ImVec4 &meshColor);
    static void InfoAndMetricsSection(const HelperStructs::GLInfo &glInfo, float framerate);

    HelperStructs::GLInfo m_pInfo;
};
