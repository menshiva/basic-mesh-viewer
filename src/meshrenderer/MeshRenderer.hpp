#pragma once

#include <cstdint>
#include <array>
#include <memory>
#include <GL/glew.h>
#include "mesh/Polygon2D.hpp"
#include "GLInfo.hpp"
#include "../config.hpp"

class MeshRenderer {
public:
    bool Init(GLInfo &infoOut);
    void Update(float deltaTime);
    static void Draw();
    void Destroy();

    void OnCurrentMeshSettingsChanged() const;
    void OnIsColorSpecifiedChanged();
    void OnSpecifiedColorChanged() const { UpdateMeshColor(m_pSpecifiedColor); }

    MeshSettings &GetCurrentMeshSettings() { return m_pCurrentMesh->GetMeshSettingsRef(); }
    bool &GetIsColorSpecifiedRef() { return m_pIsColorSpecified; }
    float *GetSpecifiedColorRef() { return &m_pSpecifiedColor.x; }
private:
    static bool FillGLInfo(GLInfo &infoOut);

    static bool CompileShader(GLenum shaderType, std::string_view path, GLuint &shaderIdOut);
    static bool CreateProgram(std::string_view path, GLuint &programIdOut);

    void UpdateMeshColor(const glm::vec3 &color) const;

#if !NDEBUG
    static void GLAPIENTRY OnGlError(GLenum, GLenum, GLuint, GLenum severity, GLsizei, const GLchar *msg, const void*);
#endif

    std::unique_ptr<Mesh> m_pCurrentMesh = std::make_unique<Polygon2D>();
    bool m_pIsColorSpecified = Config::IS_COLOR_SPECIFIED;
    glm::vec3 m_pSpecifiedColor = Config::MESH_DEFAULT_COLOR;

    GLuint m_pProgramId;
    GLint m_pColorUniformLocation;
    GLuint m_pVao;
    std::array<GLuint, 2> m_pVboIbo;

    glm::vec3 m_pAnimColorCurrent = m_pSpecifiedColor;
    glm::vec3 m_pAnimColorTo = m_pSpecifiedColor;
    float m_pAnimInterpolant = -1.0f;
};
