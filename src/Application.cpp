// clang-format off
#include "Application.h"
#include "Game.h"
#include <GL/glew.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_video.h>
#include <cstdio>
// clang-format on

bool Application::initialize() {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        printf("Failed to init SDL: %s\n", SDL_GetError());
        return false;
    }

    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 16);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_Window *window =
        SDL_CreateWindow("LearnOpenGL", 1024, 768,
                         SDL_WINDOW_OPENGL | SDL_WINDOW_MAXIMIZED | SDL_WINDOW_RESIZABLE | SDL_WINDOW_BORDERLESS);

    if (!window) {
        printf("Failed to create SDL window: %s\n", SDL_GetError());
        SDL_Quit();
        return false;
    }

    SDL_GL_SetSwapInterval(1);
    auto glContext = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, glContext);

    auto glewResult = glewInit();
    if (glewResult != GLEW_OK) {
        printf("Failed to initialize OpenGL: %s\n", glewGetErrorString(glewResult));
        SDL_Quit();
        return false;
    }

    this->window = window;
    this->running = false;
    return true;
}

void Application::run(Game &game) {
    this->running = true;
    while (this->running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_EVENT_WINDOW_RESIZED:
                    game.resized(event.window.data1, event.window.data2);
                    break;
                case SDL_EVENT_QUIT:
                case SDL_EVENT_KEY_DOWN:
                    this->running = false;
                    break;
            }
        }

        game.update();
        game.render();
        SDL_GL_SwapWindow(this->window);
    }
}

void Application::shutdown() {
    this->running = false;
    if (this->window) {
        SDL_DestroyWindow(this->window);
    }
    SDL_Quit();
}
