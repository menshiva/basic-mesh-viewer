#include "Polygon2D.hpp"
#include <glm/gtx/rotate_vector.hpp>
#include "../../config.hpp"

Polygon2D::Polygon2D() : Mesh("2D Polygon"),
                         m_pCurrentSettings(Config::MESH_SETTINGS_POLY2D_VERTICES_DEFAULT),
                         m_pNewSettings(m_pCurrentSettings) {}

bool Polygon2D::Update() {
    if (m_pNewSettings.m_VerticesNum != m_pCurrentSettings.m_VerticesNum) {
        m_pCurrentSettings.m_VerticesNum = m_pNewSettings.m_VerticesNum;
        GeneratePolygon();
        return true;
    }
    return false;
}

void Polygon2D::GeneratePolygon() {
    m_pVertexPositions.resize(m_pCurrentSettings.m_VerticesNum + 1);
    m_pIndices.resize(m_pCurrentSettings.m_VerticesNum * 3);

    const float angle = glm::two_pi<float>() / (float) m_pCurrentSettings.m_VerticesNum;

    auto rotVec = glm::vec2(0.0f, 1.0f);
    for (int i = 0; i < m_pCurrentSettings.m_VerticesNum; ++i) {
        m_pVertexPositions[i] = rotVec;
        rotVec = glm::rotate(rotVec, -angle);
        rotVec = glm::normalize(rotVec);
    }
    m_pVertexPositions[m_pCurrentSettings.m_VerticesNum] = glm::vec2(0.0f);

    for (int i = 0; i < m_pCurrentSettings.m_VerticesNum; ++i) {
        m_pIndices[i * 3 + 0] = m_pCurrentSettings.m_VerticesNum;
        m_pIndices[i * 3 + 1] = i;
        m_pIndices[i * 3 + 2] = (i + 1) % m_pCurrentSettings.m_VerticesNum;
    }
}
