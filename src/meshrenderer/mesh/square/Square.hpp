#pragma once

#include <glm/gtc/constants.hpp>
#include "../SimpleMesh.hpp"

class Square final : public SimpleMesh {
public:
    static const char *GetName() { return "Square"; }

    void Init() override {
        const auto c = glm::one_over_root_two<float>();
        m_pVertices = {
            Mesh::Vertex{glm::vec2(-c, +c), glm::vec3(1.0f, 0.0f, 0.0f)},
            Mesh::Vertex{glm::vec2(+c, +c), glm::vec3(0.5f, 1.0f, 0.0f)},
            Mesh::Vertex{glm::vec2(+c, -c), glm::vec3(0.0f, 1.0f, 1.0f)},
            Mesh::Vertex{glm::vec2(-c, -c), glm::vec3(0.5f, 0.0f, 1.0f)},
        };
        m_pTriangles = {
            Mesh::Triangle{0, 1, 2},
            Mesh::Triangle{2, 3, 0},
        };
    }
};
