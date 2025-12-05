#include "gui.h"
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_sdl3.h>
#include <imgui.h>

Gui::Gui(SDL_Window *window, SDL_GLContext glContext) {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
  ImGui::StyleColorsDark();

  ImGui_ImplSDL3_InitForOpenGL(window, glContext);
  ImGui_ImplOpenGL3_Init("#version 460");
}

Gui::~Gui() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL3_Shutdown();
  ImGui::DestroyContext();
}

void Gui::HandleEvent(SDL_Event *event) {
  ImGui_ImplSDL3_ProcessEvent(event);
}

void Gui::AddRenderer(std::shared_ptr<GuiRenderer> renderer) {
  mRenderers.push_back(renderer);
}

void Gui::Render() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplSDL3_NewFrame();
  ImGui::NewFrame();

  ImGui::Begin("Application");
  for (auto &renderer : mRenderers) {
    renderer->Render();
  }
  ImGui::End();

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
