#include "MeshRenderer.hpp"
#include <memory>
#include <fstream>
#include <sstream>

static bool CompileShader(const GLenum Type, const std::string_view Path, GLuint &OutShaderID) {
    const auto ShaderPathWithSuffix = std::unique_ptr<char[]>(new char[Path.size() + 5 + 1]);
    sprintf(ShaderPathWithSuffix.get(), Type == GL_VERTEX_SHADER ? "%s.vert" : "%s.frag", Path.data());

    std::ifstream FileStream(ShaderPathWithSuffix.get(), std::ios::in | std::ios::binary);
    if (!FileStream.is_open()) {
        fprintf(stderr, "Failed to open shader file %s.\n", ShaderPathWithSuffix.get());
        return false;
    }
    std::stringstream FileBuff;
    FileBuff << FileStream.rdbuf();
    FileStream.close();

    const auto &FileContent = FileBuff.str();

#ifndef __EMSCRIPTEN__
    const char *ShaderSrc[2] = {
        GLSL_VERSION,
        FileContent.c_str()
    };
#else
    const char *ShaderSrc[3] = {
        GLSL_VERSION,
        "precision highp float;\n",
        FileContent.c_str()
    };
#endif

    OutShaderID = glCreateShader(Type);
    glShaderSource(OutShaderID, sizeof(ShaderSrc) / sizeof(ShaderSrc[0]), ShaderSrc, nullptr);
    glCompileShader(OutShaderID);

    GLint Result;
    glGetShaderiv(OutShaderID, GL_COMPILE_STATUS, &Result);
    if (!Result) {
        GLint MsgLen;
        glGetShaderiv(OutShaderID, GL_INFO_LOG_LENGTH, &MsgLen);
        const auto Msg = std::unique_ptr<char[]>(new char[MsgLen]);
        glGetShaderInfoLog(OutShaderID, MsgLen, &MsgLen, Msg.get());
        glDeleteShader(OutShaderID);
        printf("Failed to compile %s shader: %s\n", Type == GL_VERTEX_SHADER ? "vertex" : "fragment", Msg.get());
        return false;
    }

    return true;
}

static bool CreateProgram(const std::string_view Path, GLuint &OutProgramID) {
    OutProgramID = glCreateProgram();

    GLuint VertShaderID;
    if (!CompileShader(GL_VERTEX_SHADER, Path, VertShaderID))
        return false;
    glAttachShader(OutProgramID, VertShaderID);

    GLuint FragShaderID;
    if (!CompileShader(GL_FRAGMENT_SHADER, Path, FragShaderID))
        return false;
    glAttachShader(OutProgramID, FragShaderID);

    glLinkProgram(OutProgramID);
    glValidateProgram(OutProgramID);

    glDeleteShader(VertShaderID);
    glDeleteShader(FragShaderID);

    return true;
}

MeshRenderer::MeshRenderer() : m_pSelectedMeshIdx(0) {}

bool MeshRenderer::Init(HelperStructs::GLInfo &OutInfo) {
    if (glewInit() != GLEW_OK)
        return false;

    if (!FillGLInfo(OutInfo))
        return false;

    GLuint ProgramID;
    if (!CreateProgram("res/Basic", ProgramID))
        return false;
    glUseProgram(ProgramID);

    const float Positions[6] = {
        -0.5f, -0.5f,
        0.0f, 0.5f,
        0.5f, -0.5f
    };

    glGenBuffers(1, &m_pBufferId);
    glBindBuffer(GL_ARRAY_BUFFER, m_pBufferId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Positions), Positions, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);

    return true;
}

void MeshRenderer::Update() {
    static uint8_t PrevSelectedMeshIdx = m_pSelectedMeshIdx;
    if (PrevSelectedMeshIdx != m_pSelectedMeshIdx) {
        // selected mesh idx was changed
        PrevSelectedMeshIdx = m_pSelectedMeshIdx;
    }
}

void MeshRenderer::Draw() {
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void MeshRenderer::Destroy() {
    // TODO: glDeleteProgram();
}

bool MeshRenderer::FillGLInfo(HelperStructs::GLInfo &OutInfo) {
    const auto Vendor = (const char*) glGetString(GL_VENDOR);
    if (!Vendor)
        return false;

    const auto Renderer = (const char*) glGetString(GL_RENDERER);
    if (!Renderer)
        return false;

    const auto Version = (const char*) glGetString(GL_VERSION);
    if (!Version)
        return false;

    const auto ShadingLanguageVersion = (const char*) glGetString(GL_SHADING_LANGUAGE_VERSION);
    if (!ShadingLanguageVersion)
        return false;

    OutInfo = {Vendor, Renderer, Version, ShadingLanguageVersion};
    return true;
}
