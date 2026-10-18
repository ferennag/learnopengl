#pragma once

#include <GL/glew.h>
#include <SDL3/SDL.h>
#include <glm/glm.hpp>
#include "shader.h"
#include "model.h"
#include "camera.h"
#include "ui.h"

class Application {
  public:
    bool Initialize();
    ~Application();

    void Run();

  private:
    bool mSdlInitialized = false;
    bool mMouseGrabbed = false;
    bool mRunning = false;
    SDL_Window *mWindow = nullptr;

    UI mUi;
    UIData mSettings;
    std::unique_ptr<Camera> mCamera;

    std::shared_ptr<Shader> mShader = nullptr;
    std::unique_ptr<Model> mModel = nullptr;

    void InitializeSettings();
    void GrabMouse(bool grab);
    void ProcessEvents();
    void UpdateModel();
};
