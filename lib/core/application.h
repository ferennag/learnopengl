#pragma once

#include <GL/glew.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>
#include <string>

class Application {
protected:
  int mWidth, mHeight;

  float GetAspectRatio() const;

public:
  virtual ~Application() = default;

  virtual void HandleResize(int width, int height);

  virtual bool Load(int width, int height);

  virtual void UnLoad() = 0;

  virtual std::string GetName() const = 0;

  virtual void Update() = 0;

  virtual void Render() = 0;
};
