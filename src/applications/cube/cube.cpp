#include "cube.h"
#include "core/application.h"
#include "core/camera/fps_camera.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/trigonometric.hpp"
#include <cstddef>
#include <memory>

struct Vertex {
  glm::vec3 position;
  glm::vec3 color;
};

static const Vertex vertices[]{
    {{-0.5f, -0.5f, 0.0f}, {1.0f, 1.0f, 0.0f}},
    {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 1.0f}},
    {{0.0f, 0.5f, 0.0f}, {1.0f, 0.0f, 1.0f}},
};

bool CubeApplication::Load(int width, int height) {
  Application::Load(width, height);

  if (!mShader) {
    mShader = Shader::Load("assets/shaders/cube.vert", "assets/shaders/cube.frag");
    if (!mShader) {
      return false;
    }
  }

  if (!mCamera) {
    mCamera = std::make_unique<FPSCamera>(glm::vec3{0.0f, 0.0f, 2.0f});
    mCamera->LookAt(glm::vec3{0.0f, 0.0f, 0.0f});
  }

  mProjection = glm::perspective(glm::radians(60.0f), GetAspectRatio(), 0.1f, 100.0f);

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

void CubeApplication::Update(const KeyboardState &keyboard) {
  mCamera->HandleKeyboardEvent(keyboard);
}

void CubeApplication::Render() {
  mShader->Bind();
  mShader->UniformMat4("projection", mProjection);
  mShader->UniformMat4("view", mCamera->GetView());
  mShader->UniformMat4("model", glm::identity<glm::mat4>());

  glBindVertexArray(mVao);
  glDrawArrays(GL_TRIANGLES, 0, 3);
  glBindVertexArray(0);
  mShader->Unbind();
}
