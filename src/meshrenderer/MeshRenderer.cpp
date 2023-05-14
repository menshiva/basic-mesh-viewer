#include "MeshRenderer.hpp"
#include <memory>
#include <fstream>
#include <sstream>

static bool CompileShader(const GLenum shaderType, const std::string_view path, GLuint &shaderIdOut) {
    const auto shaderPathWithSuffix = std::unique_ptr<char[]>(new char[path.size() + 5 + 1]);
    sprintf(shaderPathWithSuffix.get(), shaderType == GL_VERTEX_SHADER ? "%s.vert" : "%s.frag", path.data());

    std::ifstream fileStream(shaderPathWithSuffix.get(), std::ios::in | std::ios::binary);
    if (!fileStream.is_open()) {
        fprintf(stderr, "Failed to open shader file %s.\n", shaderPathWithSuffix.get());
        return false;
    }
    std::stringstream fileBuff;
    fileBuff << fileStream.rdbuf();
    fileStream.close();

    const auto &fileContent = fileBuff.str();

#ifndef __EMSCRIPTEN__
    const char *ShaderSrc[2] = {
        GLSL_VERSION,
        fileContent.c_str()
    };
#else
    const char *ShaderSrc[3] = {
        GLSL_VERSION,
        "precision highp float;\n",
        fileContent.c_str()
    };
#endif

    shaderIdOut = glCreateShader(shaderType);
    glShaderSource(shaderIdOut, sizeof(ShaderSrc) / sizeof(ShaderSrc[0]), ShaderSrc, nullptr);
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

static bool CreateProgram(const std::string_view path, GLuint &programIdOut) {
    programIdOut = glCreateProgram();

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

MeshRenderer::MeshRenderer() : m_pSelectedMeshIdx(0) {}

bool MeshRenderer::Init(HelperStructs::GLInfo &infoOut) {
    if (glewInit() != GLEW_OK)
        return false;

    if (!FillGLInfo(infoOut))
        return false;

    GLuint programId;
    if (!CreateProgram("res/Basic", programId))
        return false;
    glUseProgram(programId);

    const float positions[] = {
        -0.5f, -0.5f,
        0.5f, -0.5f,
        0.5f, 0.5f,
        -0.5f, 0.5f,
    };

    const uint8_t indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    GLuint vboIbo[2];
    glGenBuffers(2, vboIbo);

    glBindBuffer(GL_ARRAY_BUFFER, vboIbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIbo[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    return true;
}

void MeshRenderer::Update() {
    static uint8_t prevSelectedMeshIdx = m_pSelectedMeshIdx;
    if (prevSelectedMeshIdx != m_pSelectedMeshIdx) {
        // selected mesh idx was changed
        prevSelectedMeshIdx = m_pSelectedMeshIdx;
    }
}

void MeshRenderer::Draw() {
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, nullptr);
}

void MeshRenderer::Destroy() {
    // TODO: glDeleteBuffers
    // TODO: glDeleteProgram();
}

bool MeshRenderer::FillGLInfo(HelperStructs::GLInfo &infoOut) {
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
