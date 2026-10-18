#pragma once

#include <GL/glew.h>
#include <SDL3/SDL.h>
#include <glm/glm.hpp>
#include "shader.h"
#include "model.h"
#include "camera.h"

struct CameraData {
    glm::mat4 projection, view;
    glm::vec3 position;
};

class Application {
  public:
    bool Initialize();
    ~Application();

    void Resize(int width, int height);
    void Run();

  private:
    bool mSdlInitialized = false;
    bool mRunning = false;
    SDL_Window *mWindow = nullptr;

    Camera mCamera;

    std::shared_ptr<Shader> mShader = nullptr;
    GLuint mCameraDataUbo;
    CameraData mCameraData;
    std::unique_ptr<Model> mModel = nullptr;

    void ProcessEvents();
    void UpdateCameraData();
};
