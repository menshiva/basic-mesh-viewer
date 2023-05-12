#pragma once

#include <cstdint>
#include <GL/glew.h>

class MeshRenderer {
public:
    MeshRenderer();

    uint8_t &GetSelectedMeshIdxRef() { return m_pSelectedMeshIdx; }

    bool Init();
    void PreRender();
    void Render();
    void Destroy();
private:
    uint8_t m_pSelectedMeshIdx;

    GLuint m_pBufferId;
};
