#pragma once

#include "../SimpleMesh.hpp"

class Triangle final : public SimpleMesh {
public:
    static const char *GetName() { return "Triangle"; }

    void Init() override {
        m_pVertices = {
            Mesh::Vertex{glm::vec2(+0.000000f, +1.0f), glm::vec3(1.0f, 0.0f, 0.0f)},
            Mesh::Vertex{glm::vec2(+0.866025f, -0.5f), glm::vec3(0.0f, 1.0f, 0.0f)},
            Mesh::Vertex{glm::vec2(-0.866025f, -0.5f), glm::vec3(0.0f, 0.0f, 1.0f)},
        };
        m_pCenter = glm::vec2(0.0f, 0.25f);
        m_pTriangles = { Mesh::Triangle{0, 1, 2} };
    }
};
