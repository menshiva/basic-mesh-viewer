#include "Polygon2D.hpp"
#include <glm/gtx/rotate_vector.hpp>
#include "../../config.hpp"

Polygon2D::Polygon2D() : Mesh("2D Polygon"),
                         m_pCurrentSettings(Config::MESH_SETTINGS_POLY2D_VERTICES),
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
    if (m_pNewSettings.m_VerticesNum == 3) {
        m_pVertexPositions = {
            glm::vec2(+0.0f, +0.5f),
            glm::vec2(+0.5f, -0.5f),
            glm::vec2(-0.5f, -0.5f),
        };
        m_pIndices = { 0, 1, 2 };
        return;
    }

    if (m_pNewSettings.m_VerticesNum == 4) {
        m_pVertexPositions = {
            glm::vec2(-0.5f, -0.5f),
            glm::vec2(+0.5f, -0.5f),
            glm::vec2(+0.5f, +0.5f),
            glm::vec2(-0.5f, +0.5f),
        };
        m_pIndices = {
            0, 1, 2,
            2, 3, 0
        };
        return;
    }

    m_pVertexPositions.resize(m_pCurrentSettings.m_VerticesNum + 1);
    m_pIndices.resize(m_pCurrentSettings.m_VerticesNum * 3);

    const float angle = glm::two_pi<float>() / (float) m_pCurrentSettings.m_VerticesNum;

    auto rotVec = glm::vec2(0.0f, 0.5f);
    if (m_pNewSettings.m_VerticesNum % 2 == 0)
        rotVec = glm::rotate(rotVec, -angle);

    for (int i = 0; i < m_pCurrentSettings.m_VerticesNum; ++i) {
        m_pVertexPositions[i] = rotVec;
        rotVec = glm::rotate(rotVec, -angle);
    }
    m_pVertexPositions[m_pCurrentSettings.m_VerticesNum] = glm::vec2(0.0f);

    for (int i = 0; i < m_pCurrentSettings.m_VerticesNum; ++i) {
        m_pIndices[i * 3 + 0] = m_pCurrentSettings.m_VerticesNum;
        m_pIndices[i * 3 + 1] = i;
        m_pIndices[i * 3 + 2] = (i + 1) % m_pCurrentSettings.m_VerticesNum;
    }
}
