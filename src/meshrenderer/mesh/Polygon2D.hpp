#pragma once

#include "Mesh.hpp"

class Polygon2D final : public Mesh {
public:
    Polygon2D();

    void Init() override;
    void Update() override;

    MeshSettings *GetSettingsPtr() override { return &m_pNewSettings; }
private:
    Polygon2DSettings m_pCurrentSettings, m_pNewSettings;
};
