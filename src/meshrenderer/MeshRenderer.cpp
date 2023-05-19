#include "MeshRenderer.hpp"
#include <fstream>
#include <sstream>
#include <glm/gtx/color_space.hpp>
#include "mesh/Polygon2D.hpp"
#include "../config.hpp"

MeshRenderer::MeshRenderer() : m_pSelectedMeshIdx(Config::SELECTED_MESH_IDX),
                               m_pIsColorSpecified(Config::IS_COLOR_SPECIFIED),
                               m_pSpecifiedColor(Config::MESH_DEFAULT_COLOR),
                               m_pMeshes({
                                    std::make_unique<Mesh>(),
                                    std::make_unique<Polygon2D>()
                               }),
                               m_pProgramId{}, m_pAspectRatioUniformLocation{}, m_pColorUniformLocation{}, m_pVao{}, m_pVboIbo{},
                               m_pNewSelectedMeshIdx(m_pSelectedMeshIdx), m_pAnimColorCurrent(m_pSpecifiedColor),
                               m_pAnimColorTo(m_pSpecifiedColor), m_pAnimInterpolant(-1.0f) {}

bool MeshRenderer::Init(GLInfo &infoOut) {
    if (glewInit() != GLEW_OK)
        return false;

#ifndef __EMSCRIPTEN__
#if !NDEBUG
    glDebugMessageCallback(OnGlError, nullptr);
#endif
#endif

    if (!FillGLInfo(infoOut))
        return false;

    if (!CreateProgram(Config::SHADER_PATH, m_pProgramId))
        return false;
    glUseProgram(m_pProgramId);

    m_pAspectRatioUniformLocation = glGetUniformLocation(m_pProgramId, "u_AspectRatio");
    m_pColorUniformLocation = glGetUniformLocation(m_pProgramId, "u_Color");

    UpdateMeshColor(m_pSpecifiedColor);
    if (!m_pIsColorSpecified) // to enable transition on start
        OnIsColorSpecifiedChanged();

    glGenVertexArrays(1, &m_pVao);
    glBindVertexArray(m_pVao);

    glGenBuffers((GLsizei) m_pVboIbo.size(), m_pVboIbo.data());

    glBindBuffer(GL_ARRAY_BUFFER, m_pVboIbo[0]);

    const auto positionAttribLocation = glGetAttribLocation(m_pProgramId, "position");
    glEnableVertexAttribArray(positionAttribLocation);
    glVertexAttribPointer(positionAttribLocation, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), nullptr);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_pVboIbo[1]);

    const auto &currentMesh = m_pMeshes[m_pSelectedMeshIdx];
    currentMesh->Init();
    UpdateBuffers();

    return true;
}

void MeshRenderer::Resize(const int w, const int h) const {
    glUniform1f(m_pAspectRatioUniformLocation, (float) h / (float) w);
}

void MeshRenderer::Update(
    const float deltaTime,
    const bool IsSelectedMeshIdxChanged, const bool IsColorSpecifiedChanged, const bool IsColorChanged
) {
    if (IsSelectedMeshIdxChanged)
        OnSelectedMeshIdxChanged();
    else if (IsColorSpecifiedChanged)
        OnIsColorSpecifiedChanged();
    else if (IsColorChanged)
        UpdateMeshColor(m_pSpecifiedColor);

    if (m_pAnimInterpolant != -1.0f) {
        // here m_pAnimColorCurrent and m_pAnimColorTo are in RGB
        m_pAnimInterpolant += deltaTime * 5.0f;
        if (m_pAnimInterpolant < 1.0f) {
            UpdateMeshColor(glm::mix(m_pAnimColorCurrent, m_pAnimColorTo, m_pAnimInterpolant));
        }
        else {
            m_pAnimInterpolant = -1.0f;
            if (!m_pIsColorSpecified) {
                m_pAnimColorCurrent = glm::hsvColor(m_pAnimColorTo);
                if (glm::isnan(m_pAnimColorCurrent.x))
                    m_pAnimColorCurrent.x = 0.0f;
                if (glm::isnan(m_pAnimColorCurrent.y))
                    m_pAnimColorCurrent.y = 0.0f;
                if (glm::isnan(m_pAnimColorCurrent.z))
                    m_pAnimColorCurrent.z = 0.0f;
            }
        }
    }

    if (m_pAnimInterpolant == -1.0f && !m_pIsColorSpecified) {
        // here m_pAnimColorCurrent is in HSV
        UpdateMeshColor(glm::rgbColor(m_pAnimColorCurrent));
        m_pAnimColorCurrent.x += deltaTime * 150.0f;
        if (m_pAnimColorCurrent.x > 360.0f)
            m_pAnimColorCurrent.x -= 360.0f;
    }
}

void MeshRenderer::Draw() const {
    const auto &currentMesh = m_pMeshes[m_pSelectedMeshIdx];
    const auto &indices = currentMesh->GetIndices();
    if (!indices.empty())
        glDrawElements(GL_TRIANGLES, (GLsizei) indices.size(), GL_UNSIGNED_BYTE, nullptr);
}

void MeshRenderer::Destroy() {
    glDeleteBuffers((GLsizei) m_pVboIbo.size(), m_pVboIbo.data());
    glDeleteVertexArrays(1, &m_pVao);
    glDeleteProgram(m_pProgramId);
}

std::vector<const char*> MeshRenderer::GetMeshesNames() const {
    std::vector<const char*> out;
    out.reserve(m_pMeshes.size());
    for (const auto &mesh : m_pMeshes)
        out.push_back(mesh->GetName().data());
    return out;
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
    const char *shaderSrc[3] = { GLSL_VERSION, "precision highp float;\n", fileContent.c_str() };
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

void MeshRenderer::UpdateMeshColor(const glm::vec3 &color) const {
    glUniform3f(m_pColorUniformLocation, color.r, color.g, color.b);
}

void MeshRenderer::UpdateBuffers() const {
    const auto &currentMesh = m_pMeshes[m_pSelectedMeshIdx];
    const auto &vertPositions = currentMesh->GetVertexPositions();
    const auto &indices = currentMesh->GetIndices();
    glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr) vertPositions.size() * (GLsizeiptr) sizeof(glm::vec2), vertPositions.data(), GL_STATIC_DRAW);
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr) indices.size() * (GLsizeiptr) sizeof(uint8_t), indices.data(), GL_STATIC_DRAW);
}

#ifndef __EMSCRIPTEN__
#if !NDEBUG
void GLAPIENTRY MeshRenderer::OnGlError(
    const GLenum, const GLenum, const GLuint, const GLenum severity,
    const GLsizei, const GLchar *msg,
    const void*
) {
    if (severity != GL_DEBUG_SEVERITY_NOTIFICATION) {
        fprintf(stderr, "GL Error: %s\n", msg);
        exit(1);
    }
}
#endif
#endif

void MeshRenderer::OnSelectedMeshIdxChanged() {
    if (m_pSelectedMeshIdx != m_pNewSelectedMeshIdx) {
        m_pMeshes[m_pSelectedMeshIdx]->Destroy();
        m_pSelectedMeshIdx = m_pNewSelectedMeshIdx;
        m_pMeshes[m_pSelectedMeshIdx]->Init();
        UpdateBuffers();
    }
}

void MeshRenderer::OnIsColorSpecifiedChanged() {
    if (!m_pIsColorSpecified) {
        m_pAnimColorCurrent = m_pSpecifiedColor;
        m_pAnimColorTo = glm::hsvColor(m_pSpecifiedColor);
        if (glm::isnan(m_pAnimColorTo.x))
            m_pAnimColorTo.x = 0.0f;
        m_pAnimColorTo.y = m_pAnimColorTo.z = 1.0f;
        m_pAnimColorTo = glm::rgbColor(m_pAnimColorTo);
    }
    else {
        m_pAnimColorCurrent = glm::rgbColor(m_pAnimColorCurrent);
        m_pAnimColorTo = m_pSpecifiedColor;
    }

    m_pAnimInterpolant = 0.0f;
}
