#pragma once

#include "imgui.h"
#include "imgui_internal.h"

namespace ToolsUI{
    bool IsWindowInDockSpace(ImGuiWindow *window, ImGuiID dockspace_id = ImGui::GetID("DockSpace"));
}