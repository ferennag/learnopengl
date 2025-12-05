#include <GL/glew.h>

#include "SDL3/SDL_log.h"
#include "SDL3/SDL_video.h"
#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

struct AppState {
  SDL_Window *window;
  SDL_GLContext glContext;
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

  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *state, SDL_Event *event) {
  switch (event->type) {
    case SDL_EVENT_QUIT: {
      return SDL_APP_SUCCESS;
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
  glClearColor(0.05f, 0.05f, 0.15f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  SDL_GL_SwapWindow(appState->window);
  return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *state, SDL_AppResult result) {
  AppState *appState = static_cast<AppState *>(state);

  if (state) {
    delete appState;
  }
}
