#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

struct CameraData {
    glm::mat4 projection, view;
    glm::vec3 position;
};

class Camera {
  public:
    Camera();
    Camera(const glm::vec3 &position);

    void HandleResize(int width, int height);
    void Update(float delta);

    void MouseMotion(float x, float y, float deltaX, float deltaY);
    void SetPosition(const glm::vec3 &position);
    void BindCameraData(int location) const;
    glm::vec3 GetPosition() const;
    glm::mat4 GetViewMatrix() const;

  private:
    glm::mat4 projection;
    glm::vec3 mPosition, mDirection, mUp, mRight;
    CameraData mCameraData;
    GLuint mCameraDataUbo;

    void UpdateCameraData();
};
