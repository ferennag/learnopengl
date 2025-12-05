#include <GL/glew.h>
#include <memory>
#include <vector>

#include "SDL3/SDL_log.h"
#include "SDL3/SDL_video.h"
#include "applications/triangle/triangle.h"
#include "core/application.h"
#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

struct AppState {
  SDL_Window *window;
  SDL_GLContext glContext;
  std::vector<std::unique_ptr<Application>> applications;

  Application *activeApplication;
};

SDL_AppResult SDL_AppInit(void **state, int argc, char **argv) {
  auto *appState = new AppState();
  *state = appState;

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

  appState->window =
      SDL_CreateWindow("Learn OpenGL", 1024, 768, SDL_WINDOW_MAXIMIZED | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
  if (!appState->window) {
    SDL_Log("Failed to create window: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  appState->glContext = SDL_GL_CreateContext(appState->window);
  if (!appState->glContext) {
    SDL_Log("Failed to create GL context: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  SDL_GL_MakeCurrent(appState->window, appState->glContext);
  glewInit();

  SDL_GL_SetSwapInterval(1);

  appState->applications.push_back(std::make_unique<TriangleApplication>());

  appState->activeApplication = appState->applications.begin()->get();
  if (!appState->activeApplication->Load()) {
    SDL_Log("Failed to initialize application %s!", appState->activeApplication->GetName().c_str());
    return SDL_APP_FAILURE;
  }

  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *state, SDL_Event *event) {
  switch (event->type) {
    case SDL_EVENT_QUIT: {
      return SDL_APP_SUCCESS;
    }
    case SDL_EVENT_WINDOW_RESIZED: {
      glViewport(0, 0, event->window.data1, event->window.data2);
      break;
    }
    case SDL_EVENT_KEY_DOWN: {
      switch (event->key.key) {
        case SDLK_ESCAPE:
          return SDL_APP_SUCCESS;
      }
      break;
    }
  }
  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *state) {
  AppState *appState = static_cast<AppState *>(state);

  appState->activeApplication->Update();

  glClearColor(0.05f, 0.05f, 0.15f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  appState->activeApplication->Render();

  SDL_GL_SwapWindow(appState->window);
  return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *state, SDL_AppResult result) {
  AppState *appState = static_cast<AppState *>(state);

  if (appState->activeApplication) {
    appState->activeApplication->UnLoad();
  }

  if (state) {
    delete appState;
  }
}
