#include "shader.h"
#include <SDL3/SDL.h>
#include <optional>
#include <sstream>

enum ShaderType {
    Vertex,
    Fragment,
};

static std::optional<GLuint> CompileShader(const std::string &name, ShaderType type) {
    std::stringstream ss;
    ss << "resources/shaders/" << name;
    switch (type) {
        case ShaderType::Vertex:
            ss << ".vert";
            break;
        case ShaderType::Fragment:
            ss << ".frag";
            break;
    }

    size_t size;
    char *code = (char *)SDL_LoadFile(ss.str().c_str(), &size);
    if (!code) {
        SDL_Log("Failed to load shader: %s", ss.str().c_str());
        return std::nullopt;
    }

    GLuint shader;
    switch (type) {
        case ShaderType::Vertex:
            shader = glCreateShader(GL_VERTEX_SHADER);
            break;
        case ShaderType::Fragment:
            shader = glCreateShader(GL_FRAGMENT_SHADER);
            break;
    }

    glShaderSource(shader, 1, (const char **)&code, (GLint *)&size);
    glCompileShader(shader);
    SDL_free(code);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        int infoLogLength;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
        auto infoLog = std::string(infoLogLength, '\0');
        glGetShaderInfoLog(shader, infoLogLength, nullptr, infoLog.data());
        SDL_Log("Failed to compile %s: %s", ss.str().c_str(), infoLog.c_str());
        glDeleteShader(shader);
        return std::nullopt;
    }

    return shader;
}

std::shared_ptr<Shader> Shader::Load(const std::string &name) {
    auto vertex = CompileShader(name, ShaderType::Vertex);
    if (!vertex) {
        return nullptr;
    }

    auto fragment = CompileShader(name, ShaderType::Fragment);
    if (!fragment) {
        glDeleteShader(*vertex);
        return nullptr;
    }

    auto program = glCreateProgram();
    glAttachShader(program, *vertex);
    glAttachShader(program, *fragment);
    glLinkProgram(program);
    glDeleteShader(*vertex);
    glDeleteShader(*fragment);

    int success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        int infoLogLength;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
        auto infoLog = std::string(infoLogLength, '\0');
        glGetProgramInfoLog(program, infoLogLength, nullptr, infoLog.data());
        SDL_Log("Failed to link %s: %s", name.c_str(), infoLog.c_str());
        glDeleteProgram(program);
        return nullptr;
    }

    return std::shared_ptr<Shader>(new Shader(program));
}

Shader::Shader(GLuint id) : mId(id) {
}

Shader::~Shader() {
    glDeleteProgram(mId);
}

void Shader::Bind() const {
    glUseProgram(mId);
}

void Shader::Unbind() const {
    glUseProgram(0);
}

void Shader::UniformMat4(const std::string &name, glm::mat4 value) {
    GLint location = glGetUniformLocation(mId, name.c_str());
    if (location < 0) {
        SDL_Log("Uniform location not found: %s", name.c_str());
        return;
    }

    glUniformMatrix4fv(location, 1, GL_FALSE, &(value[0][0]));
}
