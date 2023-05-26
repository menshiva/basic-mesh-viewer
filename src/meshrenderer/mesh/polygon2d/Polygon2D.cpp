#include "Polygon2D.hpp"
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/color_space.hpp>

bool Polygon2D::Update(MeshSettings *newSettings) {
    const auto &newPolygon2dSettings = dynamic_cast<Polygon2DSettings&>(*newSettings);
    bool updateBuffers = false;
    if (m_pSettings.m_VerticesNum != newPolygon2dSettings.m_VerticesNum) {
        GeneratePolygon(newPolygon2dSettings.m_VerticesNum);
        updateBuffers = true;
    }
    m_pSettings = newPolygon2dSettings;
    return updateBuffers;
}

void Polygon2D::GeneratePolygon(const int verticesNum) {
    GeneratePolygonImpl(verticesNum);
    GenerateColors(verticesNum);
}

void Polygon2D::GeneratePolygonImpl(const int verticesNum) {
    const float angle = glm::two_pi<float>() / (float) verticesNum;

    m_pVertices.resize(verticesNum + 1);
    m_pTriangles.resize(verticesNum);

    glm::vec2 rotVec(0.0f, 1.0f);
    glm::vec2 min(rotVec), max(rotVec);

    for (auto &vert : m_pVertices) {
        min = glm::min(min, rotVec);
        max = glm::max(max, rotVec);
        vert.m_Position = rotVec; // no need to normalize
        rotVec = glm::rotate(rotVec, -angle);
    }

    m_pVertices[verticesNum].m_Position = glm::vec2(0.0f);
    m_pCenter = (min + max) / 2.0f;

    for (int i = 0; i < verticesNum; ++i) {
        auto &tr = m_pTriangles[i];
        tr.m_A = i;
        tr.m_B = (i + 1) % verticesNum;
        tr.m_C = verticesNum;
    }
}

void Polygon2D::GenerateColors(const int verticesNum) {
    const float hsvStep = 360.0f / (float) verticesNum;
    auto hsv = glm::vec3(0.0f, 1.0f, 1.0f);
    for (int i = 0; i < verticesNum; ++i) {
        m_pVertices[i].m_Color = glm::rgbColor(hsv);
        hsv.x += hsvStep;
    }
    m_pVertices[verticesNum].m_Color = glm::vec3(0.0f);
}
