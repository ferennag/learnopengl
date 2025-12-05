#pragma once

#include "app_state.h"
#include "gui/gui_renderer.h"

class AppGuiRenderer : public GuiRenderer {
  AppState *mState;

  void RenderAppSelector();

public:
  AppGuiRenderer(AppState *state);
  void Render() override;
};
