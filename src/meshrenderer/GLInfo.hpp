#pragma once

#include <string_view>

struct GLInfo {
    std::string_view m_Vendor, m_Renderer;
    std::string_view m_Version, m_ShadingLanguageVersion;
};
