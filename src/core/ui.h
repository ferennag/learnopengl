#pragma once

#include <GL/glew.h>
#include <SDL3/SDL.h>
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include "camera.h"

struct UIData {
    std::string selectedModel;
    std::vector<std::string> models;
    glm::vec3 lightPosition;

    Camera *camera;
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
