#pragma once

#include "../../config.hpp"

struct MeshSettings {
    virtual ~MeshSettings() = default;
    glm::vec3 m_Color = Config::MESH_SETTINGS_COLOR_DEFAULT;
};

struct Polygon2DSettings : MeshSettings {
    int m_VerticesNum = Config::MESH_SETTINGS_POLY2D_VERTICES_DEFAULT;
};
