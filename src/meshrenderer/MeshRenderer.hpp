#pragma once

#include <array>
#include <memory>
#include <GL/glew.h>
#include <glm/vec3.hpp>
#include "mesh/Mesh.hpp"
#include "GLInfo.hpp"

class MeshRenderer {
public:
    MeshRenderer();

    bool Init(GLInfo &infoOut);
    void Resize(int w, int h) const;
    void Update(
        float deltaTime,
        bool IsSelectedMeshIdxChanged, bool IsSelectedMeshSettingsChanged,
        bool IsColorSpecifiedChanged, bool IsColorChanged
    );
    void Draw() const;
    void Destroy();

    int &GetSelectedMeshIdxRef() { return m_pNewSelectedMeshIdx; }
    std::vector<const char*> GetMeshesNames() const;
    MeshSettings *GetCurrentMeshSettings() const { return m_pMeshes[m_pSelectedMeshIdx]->GetSettingsPtr(); }
    bool &GetIsColorSpecifiedRef() { return m_pIsColorSpecified; }
    float *GetSpecifiedColorRef() { return &m_pSpecifiedColor.x; }
private:
    static bool FillGLInfo(GLInfo &infoOut);

    static bool CompileShader(GLenum shaderType, std::string_view path, GLuint &shaderIdOut);
    static bool CreateProgram(std::string_view path, GLuint &programIdOut);

    void UpdateMeshColor(const glm::vec3 &color) const;
    void UpdateBuffers() const;

#ifndef __EMSCRIPTEN__
#if !NDEBUG
    static void GLAPIENTRY OnGlError(GLenum, GLenum, GLuint, GLenum severity, GLsizei, const GLchar *msg, const void*);
#endif
#endif

    void OnSelectedMeshIdxChanged();
    void OnSelectedMeshSettingsChanged();
    void OnIsColorSpecifiedChanged();

    int m_pSelectedMeshIdx;
    bool m_pIsColorSpecified;
    glm::vec3 m_pSpecifiedColor;

    std::array<std::unique_ptr<Mesh>, 2> m_pMeshes;

    GLuint m_pProgramId;
    GLint m_pAspectRatioUniformLocation, m_pCenterOffsetUniformLocation, m_pColorUniformLocation;
    GLuint m_pVao;
    std::array<GLuint, 2> m_pVboIbo;

    int m_pNewSelectedMeshIdx;
    glm::vec3 m_pAnimColorCurrent;
    glm::vec3 m_pAnimColorTo;
    float m_pAnimInterpolant;
};
