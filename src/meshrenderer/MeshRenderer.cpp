#include "MeshRenderer.hpp"
#include <fstream>
#include <sstream>
#include <GL/glew.h>
#include "mesh/triangle/Triangle.hpp"
#include "mesh/polygon2d/Polygon2D.hpp"

bool MeshRenderer::Init(GLInfo &infoOut) {
    CreateMeshes();

    if (glewInit() != GLEW_OK)
        return false;

#ifndef __EMSCRIPTEN__
#if !NDEBUG
    glDebugMessageCallback([] (GLenum, GLenum, GLuint, GLenum severity, GLsizei, const GLchar *msg, const void*) {
        if (severity != GL_DEBUG_SEVERITY_NOTIFICATION) {
            fprintf(stderr, "GL Error: %s\n", msg);
            exit(1);
        }
    }, nullptr);
#endif
#endif

    if (!FillGLInfo(infoOut))
        return false;

    if (!CreateProgram(Config::SHADER_PATH, m_pProgramId))
        return false;
    glUseProgram(m_pProgramId);

    m_pAspectRatioUniformLocation = glGetUniformLocation(m_pProgramId, "u_AspectRatio");
    m_pCenterOffsetUniformLocation = glGetUniformLocation(m_pProgramId, "u_CenterOffset");
    m_pUseOverrideColorLocation = glGetUniformLocation(m_pProgramId, "u_UseOverrideColor");
    m_pOverrideColorLocation = glGetUniformLocation(m_pProgramId, "u_OverrideColor");
    m_pScaleUniformLocation = glGetUniformLocation(m_pProgramId, "u_Scale");

    glGenVertexArrays(1, &m_pVao);
    glBindVertexArray(m_pVao);

    // gen VBO _AND_ IBO
    glGenBuffers(2, &m_pVbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_pVbo);
    EnableVertexAttribPointers();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_pIbo);

    UpdateUniformsIfNeeded(m_pParams, true);
    GetSelectedMesh()->Init();
    UpdateBuffers();

    return true;
}

void MeshRenderer::Resize(const int w, const int h) const {
    glUniform1f(m_pAspectRatioUniformLocation, (float) h / (float) w);
}

void MeshRenderer::Update(const UpdateParams &NewParams, MeshSettings *NewMeshSettings) {
    if (m_pParams.m_SelectedMeshIdx != NewParams.m_SelectedMeshIdx) {
        GetSelectedMesh()->Destroy();
        m_pParams.m_SelectedMeshIdx = NewParams.m_SelectedMeshIdx;
        GetSelectedMesh()->Init();
        UpdateBuffers();
    }
    else {
        UpdateUniformsIfNeeded(NewParams);
        if (GetSelectedMesh()->Update(NewMeshSettings))
            UpdateBuffers();
    }
}

void MeshRenderer::Draw() const {
    const auto &triangles = GetSelectedMesh()->GetTriangles();
    if (!triangles.empty())
        glDrawElements(GL_TRIANGLES, (GLsizei) (triangles.size() * 3), GL_UNSIGNED_BYTE, nullptr);
}

void MeshRenderer::Destroy() {
    // delete VBO _AND_ IBO
    glDeleteBuffers(2, &m_pVbo);
    glDeleteVertexArrays(1, &m_pVao);
    glDeleteProgram(m_pProgramId);
}

void MeshRenderer::CreateMeshes() {
    const auto CreateMesh = [this] <class MeshCls> () {
        m_pMeshesNames.emplace_back(MeshCls::GetName());
        m_pMeshes.emplace_back(std::make_unique<MeshCls>());
    };

    CreateMesh.operator()<Triangle>();
    CreateMesh.operator()<Polygon2D>();
}

void MeshRenderer::EnableVertexAttribPointers() {
    constexpr static int positionAttribLocation = 0;
    glEnableVertexAttribArray(positionAttribLocation);
    glVertexAttribPointer(positionAttribLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), (GLvoid*) offsetof(Mesh::Vertex, m_Position));

    constexpr int colorAttribLocation = 1;
    glEnableVertexAttribArray(colorAttribLocation);
    glVertexAttribPointer(colorAttribLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), (GLvoid*) offsetof(Mesh::Vertex, m_Color));
}

void MeshRenderer::UpdateUniformsIfNeeded(const UpdateParams &NewParams, bool force) {
    if (force || m_pParams.m_ColorType != NewParams.m_ColorType) {
        m_pParams.m_ColorType = NewParams.m_ColorType;
        glUniform1i(m_pUseOverrideColorLocation, m_pParams.m_ColorType != UpdateParams::ColorType::HUE_DISTRIBUTE);
    }

    if (force || m_pParams.m_OverrideColor != NewParams.m_OverrideColor) {
        m_pParams.m_OverrideColor = NewParams.m_OverrideColor;
        const auto &color = m_pParams.m_OverrideColor;
        glUniform3f(m_pOverrideColorLocation, color.r, color.g, color.b);
    }

    if (force || m_pParams.m_Scale != NewParams.m_Scale) {
        m_pParams.m_Scale = NewParams.m_Scale;
        glUniform1f(m_pScaleUniformLocation, m_pParams.m_Scale);
    }
}

void MeshRenderer::UpdateBuffers() {
    const auto mesh = GetSelectedMesh();

    const auto centerOffset = mesh->GetScreenCenterOffset();
    glUniform2f(m_pCenterOffsetUniformLocation, centerOffset.x, centerOffset.y);

    const auto &vertices = mesh->GetVertices();
    glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr) vertices.size() * (GLsizeiptr) sizeof(Mesh::Vertex), vertices.data(), GL_STATIC_DRAW);

    const auto &triangles = mesh->GetTriangles();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr) triangles.size() * (GLsizeiptr) sizeof(Mesh::Triangle), triangles.data(), GL_STATIC_DRAW);

    m_pDrawInfo = DrawInfo(vertices.size(), triangles.size());
}

bool MeshRenderer::FillGLInfo(GLInfo &infoOut) {
    const auto vendor = (const char*) glGetString(GL_VENDOR);
    if (!vendor)
        return false;

    const auto renderer = (const char*) glGetString(GL_RENDERER);
    if (!renderer)
        return false;

    const auto version = (const char*) glGetString(GL_VERSION);
    if (!version)
        return false;

    const auto shadingLanguageVersion = (const char*) glGetString(GL_SHADING_LANGUAGE_VERSION);
    if (!shadingLanguageVersion)
        return false;

    infoOut = {vendor, renderer, version, shadingLanguageVersion};
    return true;
}

bool MeshRenderer::CreateProgram(const std::string_view path, GLuint &programIdOut) {
    programIdOut = glCreateProgram();
    if (!programIdOut)
        return false;

    GLuint vertShaderId;
    if (!CompileShader(GL_VERTEX_SHADER, path, vertShaderId))
        return false;
    glAttachShader(programIdOut, vertShaderId);

    GLuint fragShaderID;
    if (!CompileShader(GL_FRAGMENT_SHADER, path, fragShaderID))
        return false;
    glAttachShader(programIdOut, fragShaderID);

    glLinkProgram(programIdOut);
    glValidateProgram(programIdOut);

    glDeleteShader(vertShaderId);
    glDeleteShader(fragShaderID);

    return true;
}

bool MeshRenderer::CompileShader(const GLenum shaderType, const std::string_view path, GLuint &shaderIdOut) {
    const auto shaderPathWithSuffix = std::unique_ptr<char[]>(new char[path.size() + 5 + 1]);
    sprintf(shaderPathWithSuffix.get(), shaderType == GL_VERTEX_SHADER ? "%s.vert" : "%s.frag", path.data());

    std::ifstream fileStream(shaderPathWithSuffix.get(), std::ios::in | std::ios::binary);
    if (!fileStream.is_open()) {
        fprintf(stderr, "Failed to open shader file %s\n", shaderPathWithSuffix.get());
        return false;
    }
    std::stringstream fileBuff;
    fileBuff << fileStream.rdbuf();
    fileStream.close();

    const auto &fileContent = fileBuff.str();

#ifndef __EMSCRIPTEN__
    const char *shaderSrc[2] = { GLSL_VERSION, fileContent.c_str() };
#else
    const char *shaderSrc[3] = { GLSL_VERSION, "precision mediump float;\n", fileContent.c_str() };
#endif

    shaderIdOut = glCreateShader(shaderType);
    glShaderSource(shaderIdOut, sizeof(shaderSrc) / sizeof(shaderSrc[0]), shaderSrc, nullptr);
    glCompileShader(shaderIdOut);

    GLint result;
    glGetShaderiv(shaderIdOut, GL_COMPILE_STATUS, &result);
    if (!result) {
        GLint msgLen;
        glGetShaderiv(shaderIdOut, GL_INFO_LOG_LENGTH, &msgLen);
        const auto Msg = std::unique_ptr<char[]>(new char[msgLen]);
        glGetShaderInfoLog(shaderIdOut, msgLen, &msgLen, Msg.get());
        glDeleteShader(shaderIdOut);
        printf("Failed to compile %s shader: %s\n", shaderType == GL_VERTEX_SHADER ? "vertex" : "fragment", Msg.get());
        return false;
    }

    return true;
}
