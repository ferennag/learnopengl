#include "orbital_camera.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/trigonometric.hpp"

OrbitalCamera::OrbitalCamera() : OrbitalCamera({0.0f, 0.0f, 0.0f}, 1.0f) {
}

OrbitalCamera::OrbitalCamera(const glm::vec3 &target, const float distance) : mTarget(target), mDistance(distance) {
  mAnglePolar = 45.0f;
  mAngleAzimuth = 45.0f;
}

void OrbitalCamera::HandleMouseEvent(const glm::vec2 &relative) {
  float turnSpeed = 0.1f;

  mAngleAzimuth += relative.x * turnSpeed;
  mAnglePolar += relative.y * turnSpeed;

  if (mAnglePolar > 180.0f) {
    mAnglePolar = 180.0f;
  }

  if (mAnglePolar < 1.0f) {
    mAnglePolar = 1.0f;
  }
}

void OrbitalCamera::HandleKeyboardEvent(const KeyboardState &keyboard) {
}

glm::mat4 OrbitalCamera::GetView() const {
  auto position = GetPosition();

  return glm::lookAt(position, mTarget, {0.0f, 1.0f, 0.0f});
}

glm::vec3 OrbitalCamera::GetPosition() const {
  return glm::vec3{
      mTarget.x + mDistance * glm::sin(glm::radians(mAnglePolar)) * glm::cos(glm::radians(mAngleAzimuth)),
      mTarget.y + mDistance * glm::cos(glm::radians(mAnglePolar)),
      mTarget.z + mDistance * glm::sin(glm::radians(mAnglePolar)) * glm::sin(glm::radians(mAngleAzimuth)),
  };
}

void OrbitalCamera::LookAt(const glm::vec3 &target) {
  mTarget = target;
}

void OrbitalCamera::SetDistance(const float distance) {
  mDistance = distance;
}
