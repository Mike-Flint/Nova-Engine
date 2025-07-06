#include "ViewPort.hpp"

void WindowViewPort::draw() {
    ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse 
                                    | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | 
                                    ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoDecoration);

        ImVec2 viewportSize = ImGui::GetContentRegionAvail();

        render.draw(glm::ivec2(viewportSize.x, viewportSize.y));

        ImGui::Image(
            render.viewportFBO.textureId, 
            viewportSize,
            ImVec2(0, 1), 
            ImVec2(1, 0)
        );

    ImGui::End();


    if (once) {
        once = false;
        ImGuiID dockspace_id = ImGui::GetID("DockSpace");
        ImGui::DockBuilderRemoveNode(dockspace_id);
        ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);
        ImGui::DockBuilderSetNodeSize(dockspace_id, ImGui::GetMainViewport()->Size);

        // ImGuiID dock_right = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Right, 0.3f, nullptr, &dockspace_id);

        ImGui::DockBuilderDockWindow("Viewport", dockspace_id);
        ImGui::DockBuilderFinish(dockspace_id);
    }
}