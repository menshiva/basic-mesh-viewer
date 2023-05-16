#pragma once

#include <cstdint>
#include <array>
#include <GL/glew.h>
#include "GLInfo.hpp"

class MeshRenderer {
public:
    MeshRenderer();

    bool Init(GLInfo &infoOut);
    static void Update();
    static void Draw();
    void Destroy();

    void OnSelectedMeshIdxChanged() const;
    void OnMeshColorChanged() const;

    uint8_t &GetSelectedMeshIdxRef() { return m_pSelectedMeshIdx; }
    float *GetMeshColorRef() { return m_pMeshColor.data(); }
private:
#if !NDEBUG
    static void GLAPIENTRY OnGlError(GLenum, GLenum, GLuint, GLenum severity, GLsizei, const GLchar *msg, const void*);
#endif
    static bool FillGLInfo(GLInfo &infoOut);

    uint8_t m_pSelectedMeshIdx;
    std::array<float, 3> m_pMeshColor;

    GLuint m_pProgramId;
    GLint m_pColorUniformLocation;
    GLuint m_pVao;
    std::array<GLuint, 2> m_pVboIbo;
};
