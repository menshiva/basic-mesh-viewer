#pragma once

#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

class Mesh {
public:
    struct Vertex { glm::vec2 m_Position; glm::vec3 m_Color; };
    struct Triangle { uint8_t m_A, m_B, m_C; };

    Mesh() : m_pCenter(0.0f) {}
    virtual ~Mesh() = default;

    static const char *GetName() { return "None"; }

    virtual void Init() = 0;
    virtual bool Update(struct MeshSettings *newSettings) = 0;

    void Destroy() {
        m_pVertices.clear();
        m_pTriangles.clear();
        m_pCenter = glm::vec2(0.0f);
    }

    virtual MeshSettings *GetSettingsCopy() = 0;
    const auto &GetVertices() const { return m_pVertices; }
    const auto &GetTriangles() const { return m_pTriangles; }
    glm::vec2 GetScreenCenterOffset() const { return glm::vec2(0.0f) - m_pCenter; }
protected:
    std::vector<Vertex> m_pVertices;
    std::vector<Triangle> m_pTriangles;
    glm::vec2 m_pCenter;
};
