#include "cube.h"
#include "core/application.h"
#include <cstddef>

struct Vertex {
  glm::vec3 position;
  glm::vec3 color;
};

static const Vertex vertices[]{
    {{-0.5f, -0.5f, 0.0f}, {1.0f, 1.0f, 0.0f}},
    {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 1.0f}},
    {{0.0f, 0.5f, 0.0f}, {1.0f, 0.0f, 1.0f}},
};

bool CubeApplication::Load() {
  if (!mShader) {
    mShader = Shader::Load("assets/shaders/triangle.vert", "assets/shaders/triangle.frag");
    if (!mShader) {
      return false;
    }
  }

  glCreateVertexArrays(1, &mVao);
  glCreateBuffers(1, &mVbo);

  glBindVertexArray(mVao);
  glBindBuffer(GL_ARRAY_BUFFER, mVbo);
  glNamedBufferData(mVbo, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, position));
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, color));
  glEnableVertexAttribArray(1);

  return true;
}

void CubeApplication::UnLoad() {
  glDeleteVertexArrays(1, &mVao);
  glDeleteBuffers(1, &mVbo);
}

std::string CubeApplication::GetName() const {
  return "Basic Cube";
}

void CubeApplication::Update() {
}

void CubeApplication::Render() {
  mShader->Bind();
  glBindVertexArray(mVao);
  glDrawArrays(GL_TRIANGLES, 0, 3);
  glBindVertexArray(0);
  mShader->Unbind();
}
