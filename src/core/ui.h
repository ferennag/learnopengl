#pragma once

#include <GL/glew.h>
#include <SDL3/SDL.h>
#include <vector>
#include <string>

struct UIData {
    std::string selectedModel;
    std::vector<std::string> models;
};

class UI {
  public:
    bool Initialize();
    ~UI();

    void HandleEvent(SDL_Event *event);
    void Update();
    void Render(UIData *data);

  private:
    bool mShowDemoWindow = false;
};
