#pragma once

#include "Mesh.h"
#include <glm/glm.hpp>
#include <memory>

class Model {
  private:
    Mesh *meshes;
    unsigned int numMeshes;
    glm::mat4 transform;

    Model(Mesh *meshes, unsigned int numMeshes);

  public:
    ~Model();

    static std::shared_ptr<Model> load(const std::string &path);

    void update();
    void render() const;

    glm::mat4 getTransform() const { return transform; }
};
