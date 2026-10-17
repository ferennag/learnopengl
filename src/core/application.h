#pragma once

#include <GL/glew.h>
#include <SDL3/SDL.h>
#include "shader.h"

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
    GLuint mVao, mVbo;

    void ProcessEvents();
};
