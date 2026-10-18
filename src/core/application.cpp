#include "application.h"
#include <GL/glew.h>
#include <glm/ext.hpp>

#define BUFFER_OFFSET(offset) ((void *)(offset))

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

    static const float vertices[][2] = {
        {-0.90f, -0.90f},
        {0.85f, -0.90f},
        {-0.90f, 0.85f},
        {0.90f, -0.85f},
        {0.90f, 0.90f},
        {-0.85f, 0.90f},
    };

    mShader = Shader::Load("basic");
    if (!mShader) {
        return false;
    }

    glCreateVertexArrays(1, &mVao);
    glBindVertexArray(mVao);
    glCreateBuffers(1, &mVbo);
    glNamedBufferStorage(mVbo, sizeof(vertices), vertices, 0);
    glBindBuffer(GL_ARRAY_BUFFER, mVbo);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), BUFFER_OFFSET(0));
    glEnableVertexAttribArray(0);

    mView = glm::lookAt((glm::vec3){0.0f, 0.0f, 5.0f}, (glm::vec3){0.0f, 0.0f, 0.0f}, (glm::vec3){0.0f, 1.0f, 0.0f});

    return true;
}

Application::~Application() {
    if (mSdlInitialized) {
        glDeleteVertexArrays(1, &mVao);
        glDeleteBuffers(1, &mVbo);
        SDL_Quit();
    }
}

void Application::Resize(int width, int height) {
    mProjection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
    glViewport(0, 0, width, height);
}

void Application::Run() {
    mRunning = true;
    while (mRunning) {
        ProcessEvents();

        float background[] = {0.1f, 0.1f, 0.2f, 1.0f};
        glClearBufferfv(GL_COLOR, 0, background);

        mShader->Bind();
        mShader->UniformMat4("Projection", mProjection);
        mShader->UniformMat4("View", mView);
        glBindVertexArray(mVao);
        glDrawArrays(GL_TRIANGLES, 0, 9);
        mShader->Unbind();

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
                Resize(event.window.data1, event.window.data2);
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
