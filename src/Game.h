#pragma once

#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include <glm/glm.hpp>
#include <memory>

class Game {
  private:
    int width, height;
    std::shared_ptr<Shader> shader;
    std::shared_ptr<Mesh> mesh;
    std::shared_ptr<Texture> texture;
    glm::mat4 projection, view, model;

  public:
    Game() = default;
    ~Game() = default;

    bool initialize();
    void update();
    void render();
    void destroy();
    void resized(int width, int height);
};
