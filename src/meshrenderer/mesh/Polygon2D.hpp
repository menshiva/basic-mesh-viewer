#pragma once

#include "Mesh.hpp"

class Polygon2D final : public Mesh {
public:
    Polygon2D();

    void OnMeshSettingsChanged() override;
    MeshSettings &GetMeshSettingsRef() override { return m_pNewSettings; }
private:
    Polygon2DSettings m_pCurrentSettings, m_pNewSettings;
};
