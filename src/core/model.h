#pragma once

#include <GL/glew.h>
#include <memory>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <vector>
#include "shader.h"

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
};

struct Mesh {
    glm::mat4 transform;
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    GLuint vao, vbo, ebo;
};

class Model {
  public:
    static std::unique_ptr<Model> Load(const std::string &file);

    ~Model();
    void Draw(std::shared_ptr<Shader> shader) const;

    void Rotate(float angle, const glm::vec3 &axis);

  private:
    glm::mat4 mTransform = glm::identity<glm::mat4>();
    std::vector<Mesh> mMeshes;
    glm::vec3 mMinBounds, mMaxBounds;

    Model(const std::vector<Mesh> &meshes);
};
