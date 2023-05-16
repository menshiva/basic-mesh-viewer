#pragma once

#include <cstdint>
#include <array>
#include <GL/glew.h>
#include <glm/vec3.hpp>
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
    float *GetMeshColorRef() { return &m_pMeshColor.x; }
private:
    static bool FillGLInfo(GLInfo &infoOut);

    static bool CompileShader(GLenum shaderType, std::string_view path, GLuint &shaderIdOut);
    static bool CreateProgram(std::string_view path, GLuint &programIdOut);

#if !NDEBUG
    static void GLAPIENTRY OnGlError(GLenum, GLenum, GLuint, GLenum severity, GLsizei, const GLchar *msg, const void*);
#endif

    uint8_t m_pSelectedMeshIdx;
    glm::vec3 m_pMeshColor;

    GLuint m_pProgramId;
    GLint m_pColorUniformLocation;
    GLuint m_pVao;
    std::array<GLuint, 2> m_pVboIbo;
};
