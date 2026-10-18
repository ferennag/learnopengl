#pragma once

#include <GL/glew.h>
#include <SDL3/SDL.h>

class UI {
  public:
    bool Initialize();
    ~UI();

    void HandleEvent(SDL_Event *event);
    void Update();
    void Render();

  private:
    bool mShowDemoWindow = true;
};
