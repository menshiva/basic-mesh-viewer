#pragma once

#include <cstdint>

struct MeshSettings {
    virtual ~MeshSettings() = default;
};

struct Polygon2DSettings : MeshSettings {
    explicit Polygon2DSettings(int verticesNum) : m_VerticesNum(verticesNum) {}
    int m_VerticesNum;
};
