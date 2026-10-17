#pragma once

#include <GL/glew.h>
#include <memory>
#include <string>

class Shader {
  public:
    static std::shared_ptr<Shader> Load(const std::string &name);
    ~Shader();

    void Bind() const;
    void Unbind() const;

  private:
    GLuint mId;

    Shader(GLuint id);
};
