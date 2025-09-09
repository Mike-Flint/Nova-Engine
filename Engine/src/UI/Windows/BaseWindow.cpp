#include "BaseWindow.hpp"

void BaseWindow::CloseWindow(){
    m_WindowToggle.Dock = true;
    m_WindowToggle.Show = false;
    m_WindowToggle.NeedsSettingsDock = true;
}

void BaseWindow::Draw(){
    ImGuiID dockSpace_id = ImGui::GetID("DockSpace");
        ImGuiDockNode* node = ImGui::DockBuilderGetNode(dockSpace_id); 

        if (node && m_WindowToggle.NeedsSettingsDock){
            ImGuiID left_id, right_id;
            ImGui::DockBuilderSplitNode(dockSpace_id, ImGuiDir_Left, 0.25f, &left_id, &right_id);
            ImGui::DockBuilderFinish(dockSpace_id);

            ImGui::SetNextWindowDockID(left_id, ImGuiCond_Appearing);

            m_WindowToggle.NeedsSettingsDock = false;
        }

        ImGui::Begin(m_windowName.data());

            if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup) &&
                ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
                ImGui::OpenPopup((std::string("ContextMenu") += m_windowName).c_str());
            }

            if (ImGui::BeginPopup((std::string("ContextMenu") += m_windowName).c_str())) {
                if (ImGui::MenuItem("Close")) {
                    CloseWindow();
                }

                if (m_WindowToggle.Dock) {
                    if (ImGui::MenuItem("Undock")) {
                        m_WindowToggle.Dock = false;
                        ImGuiWindow* window = ImGui::FindWindowByName(m_windowName.data());
                        if (window) {
                            ImGui::SetWindowDock(window, 0, ImGuiCond_Always);
                        }
                    }
                }
                ImGui::EndPopup();
            }

            bool isDocked = ToolsUI::IsWindowInDockSpace(ImGui::GetCurrentWindow(), dockSpace_id);
            if (isDocked && !m_prevDocked) {
                m_WindowToggle.Dock = true;
            }
            m_prevDocked = isDocked;

            DrawContent();

        ImGui::End();
}