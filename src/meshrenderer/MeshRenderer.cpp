#include "MeshRenderer.hpp"
#include <string_view>

static GLuint CompileAttachShader(const GLenum Type, const std::string_view Shader, const GLuint ProgramId) {
    const GLuint ID = glCreateShader(Type);
    const auto ShaderPathPtr = Shader.data();
    glShaderSource(ID, 1, &ShaderPathPtr, nullptr);
    glCompileShader(ID);

    int Result;
    glGetShaderiv(ID, GL_COMPILE_STATUS, &Result);
    if (!Result) {
        int MsgLen;
        glGetShaderiv(ID, GL_INFO_LOG_LENGTH, &MsgLen);
        char *Msg = (char*) alloca(MsgLen * sizeof(char));
        glGetShaderInfoLog(ID, MsgLen, &MsgLen, Msg);

        const char *ShaderType;
        if (Type == GL_VERTEX_SHADER)
            ShaderType = "vertex";
        else
            ShaderType = "fragment";

        glDeleteShader(ID);
        printf("Failed to compile %s shader: %s\n", ShaderType, Msg);

        exit(1);
    }

    glAttachShader(ProgramId, ID);
    return ID;
}

static GLuint CreateShader(const std::string_view VertexShader, const std::string_view FragmentShader) {
    const GLuint ProgramId = glCreateProgram();

    const auto VSid = CompileAttachShader(GL_VERTEX_SHADER, VertexShader, ProgramId);
    const auto FSid = CompileAttachShader(GL_FRAGMENT_SHADER, FragmentShader, ProgramId);

    glLinkProgram(ProgramId);
    glValidateProgram(ProgramId);

    glDeleteShader(VSid);
    glDeleteShader(FSid);

    return ProgramId;
}

MeshRenderer::MeshRenderer() : m_pSelectedMeshIdx(0) {}

bool MeshRenderer::Init() {
    if (glewInit() != GLEW_OK)
        return false;

    const float positions[6] = {
        -0.5f, -0.5f,
        0.0f, 0.5f,
        0.5f, -0.5f
    };

    glGenBuffers(1, &m_pBufferId);
    glBindBuffer(GL_ARRAY_BUFFER, m_pBufferId);
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);

#ifndef __EMSCRIPTEN__
    const auto ShaderId = CreateShader(
        "#version 330 core\n"
        "in vec4 position;\n"
        "void main() {\n"
        "   gl_Position = position;\n"
        "}\n",
        "#version 330 core\n"
        "out vec4 color;\n"
        "void main() {\n"
        "   color = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
        "}\n"
    );
#else
    const auto ShaderId = CreateShader(
        "#version 300 es\n"
        "in vec4 position;\n"
        "void main() {\n"
        "   gl_Position = position;\n"
        "}\n",
        "#version 300 es\n"
        "precision highp float;\n"
        "out vec4 color;\n"
        "void main() {\n"
        "   color = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
        "}\n"
    );
#endif

    glUseProgram(ShaderId);
    return true;
}

void MeshRenderer::PreRender() {
    static uint8_t PrevSelectedMeshIdx = m_pSelectedMeshIdx;
    if (PrevSelectedMeshIdx != m_pSelectedMeshIdx) {
        // selected mesh idx was changed
        PrevSelectedMeshIdx = m_pSelectedMeshIdx;
    }
}

void MeshRenderer::Render() {
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void MeshRenderer::Destroy() {
    // TODO: glDeleteProgram();
}
