#pragma once

#include <cstdint>
#include <array>
#include <GL/glew.h>
#include "../helper.hpp"

class MeshRenderer {
public:
    MeshRenderer();

    bool Init(HelperStructs::GLInfo &infoOut);
    static void Update();
    static void Draw();
    void Destroy();

    void OnSelectedMeshIdxChanged() const;
    void OnMeshColorChanged() const;

    uint8_t &GetSelectedMeshIdxRef() { return m_pSelectedMeshIdx; }
    float *GetMeshColorRef() { return m_pMeshColor.data(); }
private:
    static bool FillGLInfo(HelperStructs::GLInfo &infoOut);

    uint8_t m_pSelectedMeshIdx;
    std::array<float, 3> m_pMeshColor;

    GLuint m_pProgramId;
    GLint m_ColorUniformLocation;
    std::array<GLuint, 2> m_pVboIbo;
};
