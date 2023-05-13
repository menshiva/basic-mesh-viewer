#pragma once

#include <string_view>

#ifndef __EMSCRIPTEN__
#define GLSL_VERSION "#version 330 core\n"
#else
#define GLSL_VERSION "#version 300 es\n"
#endif

namespace HelperStructs {
    struct GLInfo {
        std::string_view m_Vendor, m_Renderer;
        std::string_view m_Version, m_ShadingLanguageVersion;
    };
}
