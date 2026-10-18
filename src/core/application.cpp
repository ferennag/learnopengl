#include "application.h"
#include <GL/glew.h>
#include <glm/ext.hpp>
#include "model.h"
#include "keyboard.h"

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

    SDL_GL_SetSwapInterval(1);

    mShader = Shader::Load("gooch");
    if (!mShader) {
        return false;
    }

    mShader->Bind();
    glCreateBuffers(1, &mCameraDataUbo);
    glBindBuffer(GL_UNIFORM_BUFFER, mCameraDataUbo);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(CameraData), nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, mCameraDataUbo);

    mCamera.SetPosition(glm::vec3(0.0f, 20.0f, 400.0f));

    mCameraData.view = mCamera.GetViewMatrix();

    mModel = Model::Load("dragon.obj");
    if (!mModel) {
        return false;
    }

    SDL_SetWindowRelativeMouseMode(mWindow, true);

    return true;
}

Application::~Application() {
    if (mSdlInitialized) {
        glDeleteBuffers(1, &mCameraDataUbo);
        SDL_Quit();
    }
}

void Application::Resize(int width, int height) {
    mCameraData.projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 1000.0f);
    glViewport(0, 0, width, height);
    UpdateCameraData();
}

void Application::UpdateCameraData() {
    mCameraData.position = mCamera.GetPosition();
    mCameraData.view = mCamera.GetViewMatrix();
    glBindBuffer(GL_UNIFORM_BUFFER, mCameraDataUbo);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(CameraData), &mCameraData);
}

void Application::Run() {
    mRunning = true;

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);

    auto previous = SDL_GetTicks();
    while (mRunning) {
        // Events
        ProcessEvents();
        auto current = SDL_GetTicks();
        float delta = ((float)(current - previous)) / 1000.0f;
        previous = current;

        // Update
        mCamera.Update(delta);
        UpdateCameraData();

        // Render
        float background[] = {0.1f, 0.1f, 0.2f, 1.0f};
        float depth = 1.0f;
        glClearBufferfv(GL_COLOR, 0, background);
        glClearBufferfv(GL_DEPTH, 0, &depth);

        // mModel->Rotate(glm::radians(10.0f * delta), glm::vec3(0.0f, 1.0f, 0.0f));

        mShader->Bind();
        mModel->Draw(mShader);
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
            case SDL_EVENT_MOUSE_MOTION: {
                mCamera.MouseMotion(event.motion.x, event.motion.y, event.motion.xrel, event.motion.yrel);
                break;
            }
            case SDL_EVENT_KEY_UP:
            case SDL_EVENT_KEY_DOWN: {
                Keyboard::GetInstance().HandleEvent(&event.key);
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
