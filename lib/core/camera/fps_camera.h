#pragma once

#include "core/camera/camera.h"
#include <SDL3/SDL.h>
#include <glm/glm.hpp>

class FPSCamera : public Camera {
public:
  FPSCamera(const glm::vec3 &position);
  virtual ~FPSCamera() = default;

  void HandleMouseEvent(const glm::vec2 &relative) override;
  void HandleKeyboardEvent(const KeyboardState &keyboard) override;

  glm::mat4 GetView() const override;
  glm::vec3 GetPosition() const override;

  void LookAt(const glm::vec3 &target) override;

private:
  glm::vec3 mPosition, mFront, mUp;
  float mYaw, mPitch;
  glm::mat4 mView;
};
