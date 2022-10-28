#include "gui/pause_scene.h"
#include "application.h"
#include <imgui.h>

namespace yc::gui {

PauseScene::PauseScene(Application* application):
    isResumeButtonHovered(false),
    isBackButtonHovered(false),
    application(application) {
}

void PauseScene::render() {

    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0.7));
    ImGui::Begin("pause", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize);

    ImGui::End();
    ImGui::PopStyleColor();
    
    auto io = ImGui::GetIO();
    ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x * 0.5f, 350), ImGuiCond_Always, ImVec2(0.5f,0.5f));
    ImGui::Begin("pause_menu", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground);
    
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));

    // RESUME BUTTON
    GLuint resumeButtonTex = isResumeButtonHovered
        ? Resource::HoveredResumeButtonTexture.getId()
        : Resource::ResumeButtonTexture.getId();

    bool isResumeButtonClicked = ImGui::ImageButton((void*) (intptr_t) resumeButtonTex, ImVec2(64*6, 16*6));
    isResumeButtonHovered = ImGui::IsItemHovered();

    if (isResumeButtonClicked) {
        application->resumeGame();
    }

    // BACK BUTTON
    GLuint backButtonTex = isBackButtonHovered
        ? Resource::HoveredBackButtonTexture.getId()
        : Resource::BackButtonTexture.getId();

    bool isBackButtonClicked = ImGui::ImageButton((void*) (intptr_t) backButtonTex, ImVec2(64*6, 16*6));
    isBackButtonHovered = ImGui::IsItemHovered();

    if (isBackButtonClicked) {
        application->stop();
    }

    ImGui::PopStyleColor(3);

    ImGui::End();
}

}