#pragma once

#include "../Mesh.hpp"
#include "../MeshSettings.hpp"

class Polygon2D final : public Mesh {
public:
    static const char *GetName() { return "2D Polygon (rotation generated)"; }

    void Init() override { GeneratePolygon(m_pSettings.m_VerticesNum); }
    bool Update(MeshSettings *newSettings) override;

    MeshSettings *GetSettingsCopy() override {
        return new Polygon2DSettings(m_pSettings);
    }
private:
    void GeneratePolygon(int verticesNum);
    void GeneratePolygonImpl(int verticesNum);
    void GenerateColors(int verticesNum);

    Polygon2DSettings m_pSettings;
};
