#pragma once

#include "gui/gui_renderer.h"
#include <SDL3/SDL.h>
#include <memory>
#include <vector>

class Gui {
private:
  std::vector<std::shared_ptr<GuiRenderer>> mRenderers;

public:
  Gui(SDL_Window *window, SDL_GLContext glContext);

  ~Gui();

  void AddRenderer(std::shared_ptr<GuiRenderer> renderer);

  void HandleEvent(SDL_Event *event);

  void Render();
};
