#pragma once

#include "core/camera/camera.h"
#include "glm/ext/vector_float3.hpp"

class OrbitalCamera : public Camera {
  glm::vec3 mTarget;
  float mDistance;
  float mAnglePolar, mAngleAzimuth;

public:
  OrbitalCamera();
  OrbitalCamera(const glm::vec3 &target, const float distance);
  ~OrbitalCamera() = default;

  void HandleMouseEvent(const glm::vec2 &relative) override;
  void HandleKeyboardEvent(const KeyboardState &keyboard) override;

  glm::mat4 GetView() const override;
  glm::vec3 GetPosition() const override;

  void LookAt(const glm::vec3 &target) override;
  void SetDistance(const float distance);
};
