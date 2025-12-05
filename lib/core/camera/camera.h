#pragma once

#include "core/keyboard.h"
#include "glm/ext/matrix_float4x4.hpp"

class Camera {

public:
  virtual ~Camera() = default;

  virtual void HandleMouseEvent(const glm::vec2 &relative) = 0;
  virtual void HandleKeyboardEvent(const KeyboardState &keyboard) = 0;

  virtual glm::mat4 GetView() const = 0;
  virtual glm::vec3 GetPosition() const = 0;

  virtual void LookAt(const glm::vec3 &target) = 0;
};
