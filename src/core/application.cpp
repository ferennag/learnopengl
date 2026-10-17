#include "application.h"
#include <GL/glew.h>

bool Application::Initialize() {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("SDL Error: %s", SDL_GetError());
        mSdlInitialized = false;
        return false;
    } else {
        mSdlInitialized = true;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);
    mWindow = SDL_CreateWindow("Opengl", 800, 600, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
    if (!mWindow) {
        SDL_Log("SDL Error: %s", SDL_GetError());
        return false;
    }

    auto context = SDL_GL_CreateContext(mWindow);
    if (!context) {
        SDL_Log("SDL Error: %s", SDL_GetError());
        return false;
    }

    if (!SDL_GL_MakeCurrent(mWindow, context)) {
        SDL_Log("SDL Error: %s", SDL_GetError());
        return false;
    }

    auto glewResult = glewInit();
    if (glewResult != GLEW_OK) {
        SDL_Log("GLEW init error: %s", glewGetErrorString(glewResult));
        return false;
    }

    return true;
}

Application::~Application() {
    if (mSdlInitialized) {
        SDL_Quit();
    }
}

void Application::Run() {
    mRunning = true;
    while (mRunning) {
        ProcessEvents();
        glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        SDL_GL_SwapWindow(mWindow);
    }
}

void Application::ProcessEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_EVENT_QUIT: {
                mRunning = false;
                break;
            }
            case SDL_EVENT_WINDOW_RESIZED: {
                break;
            }
            case SDL_EVENT_KEY_DOWN: {
                switch (event.key.key) {
                    case SDLK_ESCAPE:
                        mRunning = false;
                        break;
                }
                break;
            }
        }
    }
}
