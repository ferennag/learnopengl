#include "Game.h"
#include <GL/glew.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/quaternion_transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

bool Game::initialize() {
    shader = Shader::load("resources/shaders/simple.vert", "resources/shaders/simple.frag");
    dragon = Model::load("resources/models/xyzrgb_dragon.obj");
    view = glm::lookAt(glm::vec3{0.0f, 0.0f, -200.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f});
    return true;
}

void Game::update() { dragon->update(); }

void Game::render() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    glClearColor(0.1f, 0.01f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader->bind();
    auto mvp = projection * view * dragon->getTransform();
    shader->uniformMat4("mvp", mvp);
    dragon->render();
    shader->unbind();
}

void Game::destroy() {}

void Game::resized(int width, int height) {
    glViewport(0, 0, width, height);
    this->width = width;
    this->height = height;
    this->projection = glm::perspective(glm::radians(60.0f), (float)width / (float)height, 0.1f, 1000.0f);
}
