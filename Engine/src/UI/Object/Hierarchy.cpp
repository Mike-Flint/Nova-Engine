#include "Hierarchy.hpp"

void WindowHierarchy::draw(){

    ImGuiID dockspace_id = ImGui::GetID("DockSpace");
    ImGuiDockNode* node = ImGui::DockBuilderGetNode(dockspace_id); 

    if (node && once){
        once = false;
        ImGuiID left_id, right_id;
        ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Left, 0.25f, &left_id, &right_id);
        ImGui::DockBuilderFinish(dockspace_id);

        ImGui::SetNextWindowDockID(left_id, ImGuiCond_Once);
    }

    ImGui::Begin("Hierarchy");
    ImGui::Text("Give ME");
    ImGui::End();
}