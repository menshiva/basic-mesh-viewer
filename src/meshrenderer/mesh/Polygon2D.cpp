#include "Polygon2D.hpp"

Polygon2D::Polygon2D() : m_pCurrentSettings(Polygon2DSettings(3)), m_pNewSettings(m_pCurrentSettings) {
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
}

void Polygon2D::OnMeshSettingsChanged() {
    // TODO: process new settings
    m_pCurrentSettings = m_pNewSettings;
}
