#pragma once

#include <cstdint>
#include <GL/glew.h>
#include "../helper.hpp"

class MeshRenderer {
public:
    MeshRenderer();

    uint8_t &GetSelectedMeshIdxRef() { return m_pSelectedMeshIdx; }

    bool Init(HelperStructs::GLInfo &OutInfo);
    void Update();
    void Draw();
    void Destroy();
private:
    static bool FillGLInfo(HelperStructs::GLInfo &OutInfo);

    uint8_t m_pSelectedMeshIdx;

    GLuint m_pBufferId;
};
