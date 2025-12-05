#include <GL/glew.h>
#include <memory>
#include <vector>

#include "SDL3/SDL_log.h"
#include "SDL3/SDL_video.h"
#include "app_gui.h"
#include "app_state.h"
#include "applications/cube/cube.h"
#include "applications/triangle/triangle.h"
#include "gui/gui.h"
#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

SDL_AppResult SDL_AppInit(void **state, int argc, char **argv) {
  auto *appState = new AppState();
  *state = appState;

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

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
  appState->applications.push_back(std::make_unique<CubeApplication>());

  // -1 will force the app to reload the selected application
  appState->selectedApplication = 0;
  appState->currentApplication = -1;

  appState->gui = std::make_unique<Gui>(appState->window, appState->glContext);
  auto guiRenderer = std::make_shared<AppGuiRenderer>(appState);
  appState->gui->AddRenderer(guiRenderer);

  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *state, SDL_Event *event) {
  AppState *appState = static_cast<AppState *>(state);

  appState->gui->HandleEvent(event);

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

  if (!appState->UpdateCurrentApplication()) {
    return SDL_APP_FAILURE;
  }

  glClearColor(0.05f, 0.05f, 0.15f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  appState->CurrentApplication()->Update();
  appState->CurrentApplication()->Render();
  appState->gui->Render();

  SDL_GL_SwapWindow(appState->window);
  return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *state, SDL_AppResult result) {
  AppState *appState = static_cast<AppState *>(state);

  if (appState->CurrentApplication()) {
    appState->CurrentApplication()->UnLoad();
  }

  if (state) {
    delete appState;
  }
}
