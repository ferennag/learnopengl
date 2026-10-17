#pragma once

#include <GL/glew.h>
#include <SDL3/SDL.h>

class Application {
  public:
    bool Initialize();
    ~Application();

    void Run();

  private:
    bool mSdlInitialized = false;
    bool mRunning = false;
    SDL_Window *mWindow = nullptr;

    void ProcessEvents();
};
