#pragma once

#include <GL/glew.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>
#include <string>

class Application {
public:
  virtual ~Application() = default;

  virtual bool Load() = 0;

  virtual void UnLoad() = 0;

  virtual std::string GetName() const = 0;

  virtual void Update() = 0;

  virtual void Render() = 0;
};
