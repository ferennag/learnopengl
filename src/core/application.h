#pragma once

#include <GL/glew.h>
#include <SDL3/SDL.h>
#include <glm/glm.hpp>
#include "shader.h"
#include "model.h"

struct CameraData {
    glm::mat4 projection, view;
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

    std::shared_ptr<Shader> mShader = nullptr;
    GLuint mCameraDataUbo;
    CameraData mCameraData;
    std::unique_ptr<Model> mModel = nullptr;

    void ProcessEvents();
};
