#pragma once

#include <string>
#include <vector>
#include <glm/vec2.hpp>
#include "MeshSettings.hpp"

class Mesh {
public:
    explicit Mesh(std::string_view name = "None") : m_pName(name) {}
    virtual ~Mesh() = default;

    std::string_view GetName() const { return m_pName; }
    const std::vector<glm::vec2> &GetVertexPositions() const { return m_pVertexPositions; }
    const std::vector<uint8_t> &GetIndices() const { return m_pIndices; }

    virtual void Init() {}
    virtual bool Update() { return false; }

    void Destroy() {
        m_pVertexPositions.clear();
        m_pIndices.clear();
    }

    virtual MeshSettings *GetSettingsPtr() { return nullptr; }
protected:
    std::string m_pName;
    std::vector<glm::vec2> m_pVertexPositions;
    std::vector<uint8_t> m_pIndices;
};
