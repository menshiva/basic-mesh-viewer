#pragma once

namespace Config {
    constexpr static inline int         WIN_MIN_WIDTH = 1280;
    constexpr static inline int         WIN_MIN_HEIGHT = 720;
    constexpr static inline int         WIN_DEFAULT_WIDTH = 1280;
    constexpr static inline int         WIN_DEFAULT_HEIGHT = 720;
    constexpr static inline const char *WIN_TITLE = "Basic Mesh Viewer";
    constexpr static inline bool        WIN_DEFAULT_MAXIMIZED = true;

    constexpr static inline float       BACKGROUND_COLOR[3] = {0.36f, 0.6f, 0.65f};
    constexpr static inline float       MESH_DEFAULT_COLOR[4] = {0.36f, 0.6f, 0.65f, 1.0f}; // TODO
}
