#include "ContentBrowser.hpp"


void WindowContentBrowser::draw(){
    ImGuiID dockspace_id = ImGui::GetID("DockSpace");
    ImGuiDockNode* node = ImGui::DockBuilderGetNode(dockspace_id); 

    if (node && once){
        once = false;
        ImGuiID left_id, right_id;
        ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Left, 0.25f, &left_id, &right_id);
        ImGui::DockBuilderFinish(dockspace_id);

        ImGui::SetNextWindowDockID(left_id, ImGuiCond_Once);
    }

    ImGui::Begin("Content Browser");
        if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup) &&
            ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
            ImGui::OpenPopup("TitleBarContextMenu");
        }

        ImGui::Text("File");

        if (ImGui::BeginPopup("TitleBarContextMenu")) {
            if (ImGui::MenuItem("Option 1")) {
                // дія 1
            }
            if (ImGui::MenuItem("Option 2")) {
                // дія 2
            }
            ImGui::EndPopup();
        }
    ImGui::End();
}