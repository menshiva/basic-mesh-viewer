#pragma once

#include <string>
#include <vector>
#include <glm/common.hpp>
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

    glm::vec2 GetCenterOffset() const {
        glm::vec2 min(0.0f), max(0.0f);
        if (!m_pVertexPositions.empty()) {
            min = max = m_pVertexPositions[0];
            for (uint32_t i = 1; i < m_pVertexPositions.size(); ++i) {
                min = glm::min(min, m_pVertexPositions[i]);
                max = glm::max(max, m_pVertexPositions[i]);
            }
        }
        return glm::vec2(0.0f) - (min + max) / 2.0f;
    }

    virtual MeshSettings *GetSettingsPtr() { return nullptr; }
protected:
    std::string m_pName;
    std::vector<glm::vec2> m_pVertexPositions;
    std::vector<uint8_t> m_pIndices;
};
