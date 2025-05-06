#include "Shader.h"
#include <GL/glew.h>
#include <cstdio>

static bool loadSource(const char *path, char **out, int *length) {
    FILE *fp = fopen(path, "rb");
    if (fp == NULL) {
        return false;
    }

    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    char *contents = (char *)calloc(size + 1, sizeof(char));

    char *current = contents;
    while (!feof(fp)) {
        size_t read = fread(current, sizeof(char), size, fp);
        current += read;
    }

    *out = contents;
    *length = size;

    fclose(fp);
    return true;
}

static bool compileShader(const char *path, GLuint type, unsigned int *out) {
    char *source;
    int length;
    if (!loadSource(path, &source, &length)) {
        printf("Failed to load source: %s\n", path);
        return false;
    }

    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, (const char **)&source, &length);
    glCompileShader(shader);

    int status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {
        int logLength;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
        char *infoLog = (char *)calloc(logLength + 1, sizeof(char));
        glGetShaderInfoLog(shader, logLength, NULL, infoLog);
        printf("Failed to compile shader %s: %s\n", path, infoLog);
        free(infoLog);
        free(source);
        glDeleteShader(shader);
        return false;
    }

    *out = shader;
    free(source);
    return true;
}

std::shared_ptr<Shader> Shader::load(const std::string &vertexPath, const std::string fragmentPath) {
    unsigned int vertex, fragment;

    if (!compileShader(vertexPath.c_str(), GL_VERTEX_SHADER, &vertex)) {
        return nullptr;
    }

    if (!compileShader(fragmentPath.c_str(), GL_FRAGMENT_SHADER, &fragment)) {
        glDeleteShader(vertex);
        return nullptr;
    }

    unsigned int program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    int status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status != GL_TRUE) {
        int logLength;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
        char *infoLog = (char *)calloc(logLength + 1, sizeof(char));
        glGetProgramInfoLog(program, logLength, NULL, infoLog);
        printf("Failed to compile shader: %s\n", infoLog);
        free(infoLog);
        glDeleteProgram(program);
        return nullptr;
    }

    return std::shared_ptr<Shader>(new Shader(program));
}

Shader::Shader(unsigned int id) : id(id) {}

Shader::~Shader() { glDeleteProgram(id); }

void Shader::bind() { glUseProgram(id); }

void Shader::uniformMat4(const std::string &name, const glm::mat4 &matrix) {
    int location = glGetUniformLocation(id, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, &matrix[0][0]);
}
