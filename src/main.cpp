#include <GL/glew.h>
#include <memory>
#include <vector>

#include "SDL3/SDL_events.h"
#include "SDL3/SDL_init.h"
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

  appState->width = 1024;
  appState->height = 768;
  appState->window = SDL_CreateWindow("Learn OpenGL", appState->width, appState->height,
                                      SDL_WINDOW_MAXIMIZED | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
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
  appState->selectedApplication = appState->applications.size() - 1;
  appState->currentApplication = -1;

  appState->gui = std::make_unique<Gui>(appState->window, appState->glContext);
  auto guiRenderer = std::make_shared<AppGuiRenderer>(appState);
  appState->gui->AddRenderer(guiRenderer);

  appState->mouseGrabbed = false;

  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *state, SDL_Event *event) {
  AppState *appState = static_cast<AppState *>(state);

  // Note: If this method returns true, that means ImGui is handling the mouse/keyboard event,
  // so we want to stop propagating it to the app
  if (appState->gui->HandleEvent(event)) {
    return SDL_APP_CONTINUE;
  }

  switch (event->type) {
    case SDL_EVENT_QUIT: {
      return SDL_APP_SUCCESS;
    }
    case SDL_EVENT_WINDOW_RESIZED: {
      appState->width = event->window.data1;
      appState->height = event->window.data2;
      glViewport(0, 0, event->window.data1, event->window.data2);
      if (appState->CurrentApplication()) {
        appState->CurrentApplication()->HandleResize(event->window.data1, event->window.data2);
      }
      break;
    }
    case SDL_EVENT_MOUSE_BUTTON_DOWN: {
      SDL_SetWindowRelativeMouseMode(appState->window, true);
      break;
    }
    case SDL_EVENT_MOUSE_MOTION: {
      if (appState->CurrentApplication()) {
        appState->CurrentApplication()->HandleMouseEvent(event->motion.xrel, event->motion.yrel);
      }
      break;
    }
    case SDL_EVENT_KEY_UP:
    case SDL_EVENT_KEY_DOWN: {
      bool pressed = event->key.down;
      switch (event->key.key) {
        case SDLK_Q: {
          SDL_SetWindowRelativeMouseMode(appState->window, false);
          break;
        }
        case SDLK_ESCAPE:
          return SDL_APP_SUCCESS;
        case SDLK_W:
        case SDLK_UP: {
          appState->keyboard.pressed[static_cast<int>(Key::Up)] = pressed;
          break;
        }
        case SDLK_S:
        case SDLK_DOWN: {
          appState->keyboard.pressed[static_cast<int>(Key::Down)] = pressed;
          break;
        }
        case SDLK_A:
        case SDLK_LEFT: {
          appState->keyboard.pressed[static_cast<int>(Key::Left)] = pressed;
          break;
        }
        case SDLK_D:
        case SDLK_RIGHT: {
          appState->keyboard.pressed[static_cast<int>(Key::Right)] = pressed;
          break;
        }
        case SDLK_SPACE: {
          appState->keyboard.pressed[static_cast<int>(Key::Jump)] = pressed;
          break;
        }
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

  appState->CurrentApplication()->Update(appState->keyboard);
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
