#pragma once
#include <glm/glm.hpp>
#include <memory>
#include <string>

class Shader {
  private:
    unsigned int id;

    Shader(unsigned int id);

  public:
    ~Shader();
    static std::shared_ptr<Shader> load(const std::string &vertexPath, const std::string fragmentPath);

    void bind() const;

    void unbind() const;

    void uniformMat4(const std::string &name, const glm::mat4 &matrix);
};
