#pragma once

#include "Mesh.hpp"

class Polygon2D final : public Mesh {
public:
    Polygon2D();

    void Init() override { GeneratePolygon(); }
    bool Update() override;

    MeshSettings *GetSettingsPtr() override { return &m_pNewSettings; }
private:
    void GeneratePolygon();

    Polygon2DSettings m_pCurrentSettings, m_pNewSettings;
};
