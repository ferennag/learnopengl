#pragma once

#include "Light.h"
#include "Model.h"
#include "Shader.h"
#include <glm/glm.hpp>
#include <memory>

class Game {
  private:
    int width, height;
    std::shared_ptr<Shader> shader;
    std::shared_ptr<Model> dragon;
    glm::mat4 projection;
    glm::vec3 eye;

    PointLight light;

  public:
    Game() = default;
    ~Game() = default;

    bool initialize();
    void update();
    void render();
    void destroy();
    void resized(int width, int height);
};
