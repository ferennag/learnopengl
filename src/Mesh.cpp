#include "Mesh.h"
#include <GL/glew.h>

Mesh::~Mesh() { cleanup(); }

void Mesh::initialize(Vertex *vertices, unsigned int numVertices, Face *faces, unsigned int numFaces) {
    if (this->vertices) {
        cleanup();
    }

    this->vertices = vertices;
    this->faces = faces;
    this->numVertices = numVertices;
    this->numFaces = numFaces;

    glCreateVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glCreateBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * numVertices, vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);

    glCreateBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Face) * numFaces, faces, GL_STATIC_DRAW);
}

void Mesh::cleanup() {
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    glDeleteVertexArrays(1, &vao);
    delete[] vertices;
    delete[] faces;
}

void Mesh::render() const {
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, numFaces * 3, GL_UNSIGNED_INT, 0);
}
