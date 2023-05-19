#pragma once

#include <glm/vec3.hpp>

#ifndef __EMSCRIPTEN__
#define GLSL_VERSION "#version 330 core\n"
#else
#define GLSL_VERSION "#version 300 es\n"
#endif

namespace Config {
    constexpr static inline int         WIN_MIN_WIDTH = 1280;
    constexpr static inline int         WIN_MIN_HEIGHT = 720;
    constexpr static inline int         WIN_DEFAULT_WIDTH = 1280;
    constexpr static inline int         WIN_DEFAULT_HEIGHT = 720;
    constexpr static inline const char *WIN_TITLE = "Basic Mesh Viewer";
    constexpr static inline bool        WIN_DEFAULT_MAXIMIZED = true;
    constexpr static inline glm::vec3   BACKGROUND_COLOR = glm::vec3(0.07f);

    constexpr static inline int         SELECTED_MESH_IDX = 1;
    constexpr static inline bool        IS_COLOR_SPECIFIED = false;
    constexpr static inline glm::vec3   MESH_DEFAULT_COLOR = glm::vec3(1.0f);

    constexpr static int                MESH_SETTINGS_POLY2D_VERTICES = 3;
    constexpr static int                MESH_SETTINGS_POLY2D_VERTICES_MIN = 3;
    constexpr static int                MESH_SETTINGS_POLY2D_VERTICES_MAX = 30;

    constexpr static inline const char *SHADER_PATH = "res/Basic";
}
