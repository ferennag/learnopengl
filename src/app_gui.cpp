#include "app_gui.h"
#include "imgui.h"

AppGuiRenderer::AppGuiRenderer(AppState *state) : mState(state) {
}

void AppGuiRenderer::Render() {
  ImGui::BeginGroup();
  RenderAppSelector();
  ImGui::EndGroup();
}

void AppGuiRenderer::RenderAppSelector() {
  if (ImGui::BeginCombo("App", mState->applications[mState->currentApplication]->GetName().c_str())) {
    int idx = 0;
    for (auto &app : mState->applications) {
      bool isSelected = false;
      if (ImGui::Selectable(app->GetName().c_str(), &isSelected)) {
        mState->selectedApplication = idx;
      }
      ++idx;
    }
    ImGui::EndCombo();
  }
}
