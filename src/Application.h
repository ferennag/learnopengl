#pragma once
#include "Game.h"
#include <SDL3/SDL.h>

class Application {
  private:
    SDL_Window *window;
    bool running;

  public:
    Application() = default;
    ~Application() = default;

    bool initialize();
    void shutdown();
    void run(Game &game);
};
