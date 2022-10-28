#include "game_scene.h"
#include "imgui.h"
#include "application.h"

namespace yc::gui {

GameScene::GameScene(Player* player):
    player(player) {

}

void GameScene::render() {
    ImGuiIO& io = ImGui::GetIO();
    // ImGui::SetWindowFontScale(2);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

    ImGui::SetNextWindowBgAlpha(0.2);
    ImGui::SetNextWindowPos(ImVec2(10, 10));
    ImGui::Begin("debug", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize);                       
    ImGui::Text("FPS: %.0f", 1.0f/yc::Application::GetDeltaTime());

    auto postion = player->getCamera()->getPosition();
    ImGui::Text("Position: %.0f %.0f %.0f", postion.x, postion.y, postion.z);       

    std::string selectingBlockName = "None";

    if (player->isSelectingBlock()) {
        selectingBlockName = yc::world::GetBlockName(player->getSelectingBlockType());
    }

    ImGui::Text("Block: %s", selectingBlockName.c_str());

    ImGui::End();

    ImGui::SetNextWindowBgAlpha(0.3);
    ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y - 50), ImGuiCond_Always, ImVec2(0.5f,0.5f));
    ImGui::Begin("inventory", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar);
    
    auto inventory = player->getInventory();

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(8, 8));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 3);
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0, 0, 0, 0));
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0, 0, 0, 1));

    for (int slot=0;slot<inventory.size();++slot) {
        ImVec2 size = { 40, 40 };
        GLuint id = Resource::BlockIcons[inventory[slot]].getId();

        if (slot == player->getCurrentSlot()) {
            ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.72f, 0.72f, 0.72f, 1));
            ImGui::SetNextWindowBgAlpha(0.3);
        } else {
            ImGui::SetNextWindowBgAlpha(0);
        }

        ImGui::BeginChildFrame(slot + 12345, ImVec2(56, 56), ImGuiWindowFlags_NoScrollbar);

        if (slot == player->getCurrentSlot()) {
            size = { 48, 48 };
            auto cursor = ImGui::GetCursorPos();
            ImGui::SetCursorPos(ImVec2(cursor.x-4, cursor.y-4));
        }

        ImGui::Image((void*) (intptr_t) id, size);
        ImGui::EndChildFrame();
        ImGui::SameLine();

        if (slot == player->getCurrentSlot()) {
            ImGui::PopStyleColor();
        }
    }

    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor(2);
    ImGui::End();

    ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y - 110), ImGuiCond_Always, ImVec2(0.5f,0.5f));
    ImGui::SetNextWindowBgAlpha(0.4);
    ImGui::Begin("inventory_header", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar);

    ImGui::Text(world::GetBlockName(player->getCurrentBlockType()).c_str());

    ImGui::End();

    ImGui::PopStyleVar();
}

}