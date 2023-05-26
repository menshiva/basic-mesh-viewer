#pragma once

#include <string_view>
#include <glm/vec3.hpp>

struct GLInfo {
    std::string_view m_Vendor, m_Renderer;
    std::string_view m_Version, m_ShadingLanguageVersion;
};

struct UpdateParams {
    enum class ColorType : uint8_t {
        INTERPOLATE = 0,
        HUE_DISTRIBUTE,
        SPECIFIED,
        NUM
    };

    int m_SelectedMeshIdx;
    float m_Scale;
    ColorType m_ColorType;
    glm::vec3 m_OverrideColor;
};

struct DrawInfo {
    uint32_t m_VerticesNum, m_TrianglesNum;
};
