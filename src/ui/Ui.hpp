#pragma once

#include <cstdint>
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"

class UI {
public:
    static bool Init(const char *GlslVersion, struct GLFWwindow *Window);
    static void PreRender(uint8_t &SelectedMeshIdx, ImVec4 &MeshColor);
    static void Render();
    static void Destroy();
};
