#pragma once

#include <glm/vec3.hpp>

#ifndef __EMSCRIPTEN__
#define GLSL_VERSION "#version 330 core\n"
#else
#define GLSL_VERSION "#version 300 es\n"
#endif

namespace Config {
    constexpr inline int         WIN_MIN_WIDTH = 1280;
    constexpr inline int         WIN_MIN_HEIGHT = 720;
    constexpr inline int         WIN_WIDTH_DEFAULT = 1280;
    constexpr inline int         WIN_HEIGHT_DEFAULT = 720;
    constexpr inline const char *WIN_TITLE = "Basic Mesh Viewer";
    constexpr inline bool        WIN_MAXIMIZED_DEFAULT = false;
    constexpr inline glm::vec3   BACKGROUND_COLOR = glm::vec3(0.07f);

    constexpr inline int         SELECTED_MESH_IDX_DEFAULT = 0;

    constexpr inline uint8_t     MESH_SETTINGS_COLOR_TYPE_DEFAULT = 0;
    constexpr inline glm::vec3   MESH_SETTINGS_COLOR_DEFAULT = glm::vec3(1.0f);

    constexpr inline float       MESH_SETTINGS_SCALE_DEFAULT = 0.75f;
    constexpr inline float       MESH_SETTINGS_SCALE_MIN = 0.01f;
    constexpr inline float       MESH_SETTINGS_SCALE_MAX = 2.0f;

    constexpr int                MESH_SETTINGS_POLY2D_VERTICES_DEFAULT = 3;
    constexpr int                MESH_SETTINGS_POLY2D_VERTICES_MIN = 3;
    constexpr int                MESH_SETTINGS_POLY2D_VERTICES_MAX = 50;

    constexpr inline const char *SHADER_PATH = "res/shaders/Basic";
}
