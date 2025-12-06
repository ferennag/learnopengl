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
  glm::vec2 uv;
};

static const Vertex vertices[]{
    {{-0.5f, -0.5f, 0.0f}, {0.0, 0.0}}, // Bottom left
    {{0.5f, -0.5f, 0.0f}, {2.0, 0.0}},  // Bottom right
    {{0.5f, 0.5f, 0.0f}, {2.0, 2.0}},   // Top right
    {{-0.5f, 0.5f, 0.0f}, {0.0f, 2.0}}, // Top left
};

static const GLuint indices[] = {0, 1, 3, 2};

static const GLubyte checkerboardTexture[] = {
    0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,

    0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF,

    0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,

    0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF,

    0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,

    0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF,

    0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,

    0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF,
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
  glCreateBuffers(1, &mEbo);

  glBindVertexArray(mVao);
  glBindBuffer(GL_ARRAY_BUFFER, mVbo);
  glNamedBufferData(mVbo, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, position));
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, uv));
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEbo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  glCreateTextures(GL_TEXTURE_2D, 1, &mTexture);
  glTextureStorage2D(mTexture, 4, GL_R8, 8, 8);
  glTextureSubImage2D(mTexture, 0, 0, 0, 8, 8, GL_RED, GL_UNSIGNED_BYTE, checkerboardTexture);
  glTextureParameteri(mTexture, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
  glTextureParameteri(mTexture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTextureParameteri(mTexture, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTextureParameteri(mTexture, GL_TEXTURE_WRAP_T, GL_REPEAT);
  // The texture only specifies the red channel, but we make sure OpenGL use that value for all other channels, so we
  // get a black and white image
  glTextureParameteri(mTexture, GL_TEXTURE_SWIZZLE_A, GL_ONE);
  glTextureParameteri(mTexture, GL_TEXTURE_SWIZZLE_G, GL_RED);
  glTextureParameteri(mTexture, GL_TEXTURE_SWIZZLE_B, GL_RED);
  glGenerateTextureMipmap(mTexture);

  return true;
}

void CubeApplication::UnLoad() {
  glDeleteVertexArrays(1, &mVao);
  glDeleteBuffers(1, &mVbo);
  glDeleteBuffers(1, &mEbo);
  glDeleteTextures(1, &mTexture);
}

std::string CubeApplication::GetName() const {
  return "Basic Cube";
}

void CubeApplication::HandleMouseEvent(float relX, float relY) {
  mCamera->HandleMouseEvent(glm::vec2{relX, relY});
}

void CubeApplication::Update(const KeyboardState &keyboard) {
  mCamera->HandleKeyboardEvent(keyboard);
}

void CubeApplication::Render() {
  mShader->Bind();
  glBindTexture(GL_TEXTURE_2D, mTexture);
  mShader->UniformMat4("projection", mProjection);
  mShader->UniformMat4("view", mCamera->GetView());
  mShader->UniformMat4("model", glm::identity<glm::mat4>());

  glBindVertexArray(mVao);
  glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, nullptr);
  glBindVertexArray(0);
  mShader->Unbind();
}
