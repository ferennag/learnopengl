#pragma once

#include "SDL3/SDL_video.h"
#include "core/application.h"
#include "core/keyboard.h"
#include "gui/gui.h"
#include <memory>
#include <vector>

struct AppState {
  KeyboardState keyboard;
  SDL_Window *window;
  SDL_GLContext glContext;
  int width, height;
  std::vector<std::unique_ptr<Application>> applications;
  int currentApplication;
  int selectedApplication;

  std::unique_ptr<Gui> gui;

  Application *CurrentApplication() {
    if (currentApplication < 0 || currentApplication >= applications.size()) {
      return nullptr;
    }

    return applications[currentApplication].get();
  }

  bool UpdateCurrentApplication() {
    if (currentApplication != selectedApplication) {
      if (currentApplication >= 0 && currentApplication < applications.size()) {
        applications[currentApplication]->UnLoad();
      }

      currentApplication = selectedApplication;

      if (!applications[currentApplication]->Load(width, height)) {
        SDL_Log("Failed to initialize application %s!", applications[currentApplication]->GetName().c_str());
        return false;
      }
    }
    return true;
  }
};
