#pragma once

#include <vector>
#include <glm/vec2.hpp>
#include "../MeshSettings.hpp"

class Mesh {
public:
    virtual void OnMeshSettingsChanged() = 0;

    virtual MeshSettings &GetMeshSettingsRef() = 0;
    const std::vector<glm::vec2> &GetVertexPositions() const { return m_pVertexPositions; }
    const std::vector<uint8_t> &GetIndices() const { return m_pIndices; }
protected:
    std::vector<glm::vec2> m_pVertexPositions;
    std::vector<uint8_t> m_pIndices;
};
