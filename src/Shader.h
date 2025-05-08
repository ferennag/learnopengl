#pragma once
#include "Light.h"
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

    void uniformMat3(const std::string &name, const glm::mat3 &matrix);
    void uniformMat4(const std::string &name, const glm::mat4 &matrix);
    void uniformVec3(const std::string &name, const glm::vec3 &vector);
    void uniformPointLight(const std::string &name, const PointLight &light);
};
