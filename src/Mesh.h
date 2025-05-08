#pragma once

struct Vertex {
    float position[3];
    float normal[3];
    float uv[2];
};

struct Face {
    unsigned int indices[3];
};

class Mesh {
  private:
    Vertex *vertices;
    Face *faces;
    unsigned int numVertices, numFaces;
    unsigned int vao, vbo, ebo;

  public:
    Mesh() = default;

    void initialize(Vertex *vertices, unsigned int numVertices, Face *faces, unsigned int numFaces);
    void cleanup();

    ~Mesh();

    void render() const;
};
