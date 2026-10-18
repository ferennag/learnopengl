#include "ui.h"
#include <imgui.h>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_opengl3.h>

bool UI::Initialize() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

    ImGuiStyle &style = ImGui::GetStyle();
    ImGui::StyleColorsDark();
    style.FrameRounding = 3.0f;
    style.GrabRounding = 3.0f;

    SDL_Window *window = SDL_GL_GetCurrentWindow();
    SDL_GLContext context = SDL_GL_GetCurrentContext();
    ImGui_ImplSDL3_InitForOpenGL(window, context);
    ImGui_ImplOpenGL3_Init();

    return true;
}

UI::~UI() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
}

void UI::HandleEvent(SDL_Event *event) {
    ImGui_ImplSDL3_ProcessEvent(event);
}

void UI::Update() {
}

void UI::Render(UIData *data) {
    auto &io = ImGui::GetIO();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    if (mShowDemoWindow) {
        ImGui::ShowDemoWindow(&mShowDemoWindow);
    }

    {
        ImGui::Begin("Settings");
        ImGui::Text("%.1f FPS (%.3f ms/frame)", 1000.0f / io.Framerate, io.Framerate);

        if (ImGui::CollapsingHeader("Model settings", ImGuiTreeNodeFlags_DefaultOpen)) {
            if (ImGui::BeginCombo("Model", data->selectedModel.c_str())) {
                for (const std::string &model : data->models) {
                    bool isSelected = model == data->selectedModel;
                    if (ImGui::Selectable(model.c_str(), isSelected)) {
                        data->selectedModel = model;
                    }

                    if (isSelected) {
                        ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndCombo();
            }
        }

        if (ImGui::CollapsingHeader("Camera settings", ImGuiTreeNodeFlags_DefaultOpen)) {
            float fov = data->camera->GetFov();
            if (ImGui::SliderFloat("FOV", &fov, 0.1f, 120.0f)) {
                data->camera->SetFov(fov);
            }
        }

        if (ImGui::CollapsingHeader("Light settings", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::SliderFloat3("Light Position", &data->lightPosition[0], -500, 500);
        }
        ImGui::End();
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
