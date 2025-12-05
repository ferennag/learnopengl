#pragma once

#include <core/application.h>
#include <core/shader.h>
#include <memory>

class TriangleApplication : public Application {
  GLuint mVao, mVbo;
  std::unique_ptr<Shader> mShader;

public:
  bool Load(int width, int height) override;

  void UnLoad() override;

  std::string GetName() const override;

  void Update() override;

  void Render() override;
};
