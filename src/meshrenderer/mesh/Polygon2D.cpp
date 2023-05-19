#include "Polygon2D.hpp"
#include <glm/gtx/rotate_vector.hpp>
#include "../../config.hpp"

Polygon2D::Polygon2D() : Mesh("2D Polygon"),
                         m_pCurrentSettings(Config::MESH_SETTINGS_POLY2D_VERTICES),
                         m_pNewSettings(m_pCurrentSettings) {}

void Polygon2D::Init() {
    // TODO

    //    m_pVertexPositions.resize(m_pCurrentSettings.m_VerticesNum);
    //    const float angle = glm::two_pi<float>() / (float) m_pCurrentSettings.m_VerticesNum;
    //
    //    auto rotVec = glm::vec2(0.0f, 0.5f);
    //    for (auto &vec : m_pVertexPositions) {
    //        vec = rotVec;
    //        rotVec = glm::rotate(rotVec, angle);
    //    }
    //    m_pIndices = { 0, 1, 2 };

    m_pVertexPositions = {
        glm::vec2(+0.0f, +0.5f),
        glm::vec2(+0.5f, -0.5f),
        glm::vec2(-0.5f, -0.5f),
    };
    m_pIndices = { 0, 1, 2 };

    // m_pVertexPositions = {
    //     glm::vec2(-0.5f, -0.5f),
    //     glm::vec2(+0.5f, -0.5f),
    //     glm::vec2(+0.5f, +0.5f),
    //     glm::vec2(-0.5f, +0.5f),
    // };
    // m_pIndices = {
    //     0, 1, 2,
    //     2, 3, 0
    // };
}

void Polygon2D::Update() {
    if (m_pNewSettings.m_VerticesNum != m_pCurrentSettings.m_VerticesNum) {
        // TODO
        m_pCurrentSettings.m_VerticesNum = m_pNewSettings.m_VerticesNum;
    }
}
