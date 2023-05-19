#pragma once

#include <cstdint>

struct MeshSettings {};

struct Polygon2DSettings : MeshSettings {
    explicit Polygon2DSettings(uint8_t verticesNum) : m_VerticesNum(verticesNum) {}
    uint8_t m_VerticesNum;
};
