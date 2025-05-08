#include "Game.h"
#include <GL/glew.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/quaternion_transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

bool Game::initialize() {
    shader = Shader::load("resources/shaders/simple.vert", "resources/shaders/simple.frag");
    dragon = Model::load("resources/models/xyzrgb_dragon.obj");
    eye = glm::vec3{0.0f, 0.0f, -200.0f};
    light.position = glm::vec3{0.0f, 100.0f, 0.0f};
    light.color = glm::vec3{1.0f, 1.0f, 1.0f};
    return true;
}

void Game::update() { dragon->update(); }

void Game::render() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glEnable(GL_MULTISAMPLE);

    glClearColor(0.05f, 0.01f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader->bind();
    auto view = glm::lookAt(eye, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f});
    auto vp = projection * view;
    shader->uniformMat4("viewProj", vp);
    shader->uniformMat4("model", dragon->getTransform());
    shader->uniformMat3("inverseTransposeModel", dragon->getInverseTransposeTransform());
    shader->uniformVec3("eye", eye);
    shader->uniformPointLight("light", light);
    dragon->render();
    shader->unbind();
}

void Game::destroy() {}

void Game::resized(int width, int height) {
    glViewport(0, 0, width, height);
    this->width = width;
    this->height = height;
    this->projection = glm::perspective(glm::radians(60.0f), (float)width / (float)height, 0.5f, 1000.0f);
}
