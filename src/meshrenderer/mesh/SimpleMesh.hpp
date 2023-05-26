#pragma once

#include "Mesh.hpp"
#include "MeshSettings.hpp"

class SimpleMesh : public Mesh {
public:
    bool Update(struct MeshSettings *newSettings) override {
        m_pSettings = *newSettings;
        return false; // no need to update buffers
    }

    MeshSettings *GetSettingsCopy() override {
        return new MeshSettings(m_pSettings);
    }
protected:
    MeshSettings m_pSettings;
};
