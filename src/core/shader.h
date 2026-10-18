#pragma once

#include <GL/glew.h>
#include <memory>
#include <string>
#include <glm/glm.hpp>

class Shader {
  public:
    static std::shared_ptr<Shader> Load(const std::string &name);
    ~Shader();

    void Bind() const;
    void Unbind() const;

    GLuint GetUniformBufferIndex(const std::string &name);
    void UniformMat3(const std::string &name, glm::mat3 value) const;
    void UniformMat4(const std::string &name, glm::mat4 value) const;

  private:
    GLuint mId;

    Shader(GLuint id);
};
