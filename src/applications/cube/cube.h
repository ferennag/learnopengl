#pragma once

#include "core/camera/camera.h"
#include "core/keyboard.h"
#include <core/application.h>
#include <core/shader.h>
#include <memory>

class CubeApplication : public Application {
  GLuint mVao, mVbo;
  std::unique_ptr<Shader> mShader;
  glm::mat4 mProjection;
  std::unique_ptr<Camera> mCamera;

public:
  bool Load(int width, int height) override;

  void UnLoad() override;

  std::string GetName() const override;

  void Update(const KeyboardState &keyboard) override;

  void Render() override;

  void HandleMouseEvent(float relX, float relY) override;
};
