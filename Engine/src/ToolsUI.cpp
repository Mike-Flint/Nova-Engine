#include "ToolsUI.hpp"

bool ToolsUI::IsWindowInDockSpace(ImGuiWindow* window, ImGuiID dockspace_id) {

    ImGuiDockNode* node = window && window->DockNode ? window->DockNode : nullptr;
    while (node) {
        if (node->ID == dockspace_id)
            return true;
        node = node->ParentNode;
    }
    return false;
}