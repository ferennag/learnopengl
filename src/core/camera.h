#pragma once

#include <glm/glm.hpp>

class Camera {
  public:
    Camera();
    Camera(const glm::vec3 &position);

    void Update(float delta);
    void MouseMotion(float x, float y, float deltaX, float deltaY);
    void SetPosition(const glm::vec3 &position);
    glm::vec3 GetPosition() const;
    glm::mat4 GetViewMatrix() const;

  private:
    glm::vec3 mPosition, mDirection, mUp, mRight;
};
