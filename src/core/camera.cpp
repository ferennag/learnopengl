#include "camera.h"
#include "keyboard.h"
#include <glm/ext.hpp>

#define SPEED 800.0f
#define LOOK_SENSITIVITY 0.02f

Camera::Camera() : Camera(glm::vec3(0.0f, 0.0f, 0.0f)) {
}

Camera::Camera(const glm::vec3 &position) {
    mPosition = position;
    mUp = glm::vec3(0.0f, 1.0f, 0.0f);
    mDirection = glm::normalize(glm::vec3(0.0f, 0.0f, -1.0f));
    mRight = glm::cross(mDirection, mUp);
    mFov = 45.0f;
    mAspect = 1.0f;

    glCreateBuffers(1, &mCameraDataUbo);
    glBindBuffer(GL_UNIFORM_BUFFER, mCameraDataUbo);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(CameraData), nullptr, GL_DYNAMIC_DRAW);
}

void Camera::HandleResize(int width, int height) {
    mAspect = (float)width / (float)height;
    mCameraData.projection = glm::perspective(glm::radians(mFov), mAspect, 0.1f, 1000.0f);
    glViewport(0, 0, width, height);
    UpdateCameraData();
}

void Camera::SetPosition(const glm::vec3 &position) {
    mPosition = position;
}

glm::vec3 Camera::GetPosition() const {
    return mPosition;
}

void Camera::SetFov(const float fov) {
    mFov = fov;
    mCameraData.projection = glm::perspective(glm::radians(mFov), mAspect, 0.1f, 1000.0f);
    UpdateCameraData();
}

float Camera::GetFov() const {
    return mFov;
}

void Camera::Update(float delta) {
    glm::vec3 vector = glm::vec3(0, 0, 0);
    glm::vec3 movementDirection = glm::normalize(mDirection);
    movementDirection.y = 0.0f;

    auto &keyboard = Keyboard::GetInstance();

    if (keyboard.IsPressed(Key::Up)) {
        vector += movementDirection;
    }

    if (keyboard.IsPressed(Key::Down)) {
        vector -= movementDirection;
    }

    if (keyboard.IsPressed(Key::Left)) {
        vector -= mRight;
    }

    if (keyboard.IsPressed(Key::Right)) {
        vector += mRight;
    }

    if (glm::abs(glm::length(vector)) > 0) {
        vector = glm::normalize(vector);
        vector *= delta * SPEED;
        mPosition += vector;
    }

    UpdateCameraData();
}

void Camera::BindCameraData(int location) const {
    glBindBufferBase(GL_UNIFORM_BUFFER, location, mCameraDataUbo);
}

void Camera::UpdateCameraData() {
    mCameraData.position = GetPosition();
    mCameraData.view = GetViewMatrix();
    glBindBuffer(GL_UNIFORM_BUFFER, mCameraDataUbo);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(CameraData), &mCameraData);
}

void Camera::MouseMotion(float x, float y, float deltaX, float deltaY) {
    glm::mat4 transform = glm::identity<glm::mat4>();
    transform = glm::rotate(transform, glm::radians(deltaX * -LOOK_SENSITIVITY), mUp);
    transform = glm::rotate(transform, glm::radians(deltaY * -LOOK_SENSITIVITY), mRight);
    mDirection = glm::normalize(transform * glm::vec4(mDirection, 0.0f));
    mRight = glm::normalize(transform * glm::vec4(mRight, 0.0f));
}

glm::mat4 Camera::GetViewMatrix() const {
    glm::vec3 target = mPosition + mDirection;
    return glm::lookAt(mPosition, target, mUp);
}
