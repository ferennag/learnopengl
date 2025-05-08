#pragma once

#include "Model.h"
#include "Shader.h"
#include <glm/glm.hpp>
#include <memory>

class Game {
  private:
    int width, height;
    std::shared_ptr<Shader> shader;
    std::shared_ptr<Model> dragon;
    glm::mat4 projection, view;

  public:
    Game() = default;
    ~Game() = default;

    bool initialize();
    void update();
    void render();
    void destroy();
    void resized(int width, int height);
};
