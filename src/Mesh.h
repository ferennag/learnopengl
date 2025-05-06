#pragma once

#include <memory>

class Mesh {
  private:
    unsigned int vao, vbo, numVertices;

    Mesh(unsigned int vao, unsigned int vbo, unsigned int numVertices);

  public:
    static std::shared_ptr<Mesh> createCube();

    void update();
    void render();
    ~Mesh();
};
