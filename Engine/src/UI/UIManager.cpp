#include "UIManager.hpp"

bool UIManager::areAllWindowsDocked() {

    struct {
        const char* name;
        bool& dock;
    } windows[] = {
        { "Content Browser", m_State.ContentBrowser.Dock },
        { "Details",         m_State.DetailsPanel.Dock },
        { "Hierarchy",       m_State.Hierarchy.Dock }
    };


    for (const auto& w : windows) {
        if(w.dock){
            ImGuiWindow* window = ImGui::FindWindowByName(w.name);
            if (window && !ToolsUI::IsWindowInDockSpace(window))
                return false;
        }
    }

    return true;
}

void UIManager::updateDockingLogic() {
    ImGuiWindow* winMoved = GImGui->MovingWindow;
    bool isMovingWindowNow = false;
    if (winMoved) {
        std::string_view name = winMoved->Name;
        isMovingWindowNow =
            (name == "Content Browser" && m_State.ContentBrowser.Dock) ||
            (name == "Details"         && m_State.DetailsPanel.Dock)   ||
            (name == "Hierarchy"       && m_State.Hierarchy.Dock);
    }

    if (m_CheckDockingOnNextFrame){
        m_CheckDockingOnNextFrame = false;

        if (!m_LayoutBeforeMove.empty() && !areAllWindowsDocked()){
            ImGui::LoadIniSettingsFromMemory(m_LayoutBeforeMove.c_str(), m_LayoutBeforeMove.size());
        }

        m_LayoutBeforeMove.clear();
    }

    if (!m_WasMovingWindowLastFrame && isMovingWindowNow){
        size_t data_size = 0;
        const char* layout_data = ImGui::SaveIniSettingsToMemory(&data_size);
        m_LayoutBeforeMove.assign(layout_data, data_size);
    }

    if (m_WasMovingWindowLastFrame && !isMovingWindowNow){
        m_CheckDockingOnNextFrame = true;
    }

    m_WasMovingWindowLastFrame = isMovingWindowNow;
}

void UIManager::drawDockspaceLayout(){
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(ImGui::GetStyle().FramePadding.x, 10.0f));

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking |            ImGuiWindowFlags_NoTitleBar | 
                                    ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus | 
                                    ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_MenuBar;
    
    if (!m_State.CanResize){
        window_flags |= ImGuiWindowFlags_NoResize;
    }
    ImGui::SetNextWindowSizeConstraints(ImVec2(400, 300), ImVec2(FLT_MAX, FLT_MAX));

    ImGui::Begin("DockSpace_Window", nullptr, window_flags);

    ImGui::PopStyleVar(4);

    ImGuiID dockspace_id = ImGui::GetID("DockSpace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);
}

void UIManager::syncWindowWithImGui() {
    
    ImVec2 sizeWindow = ImGui::GetWindowSize();

    if(sizeWindow.x != m_SizeWindow.x || sizeWindow.y != m_SizeWindow.y){
        m_SizeWindow = glm::ivec2(sizeWindow.x, sizeWindow.y);
        ImVec2 posWindow = ImGui::GetWindowPos();

        AppWindow::SetWindowSize(m_SizeWindow.x, m_SizeWindow.y);
        AppWindow::SetWindowPos(posWindow.x, posWindow.y);
    }
}

void UIManager::drawUI() {
    imGui.newFrame();

    drawDockspaceLayout();

        syncWindowWithImGui();

        titleBar.Draw();
        viewPort.Draw();

        if(m_State.ContentBrowser.Show) contentBrowser.Draw();
        if(m_State.DetailsPanel.Show) detailsPanel.Draw();
        if(m_State.Hierarchy.Show) hierarchy.Draw();

        updateDockingLogic();

    ImGui::End();

    imGui.render();


    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable){
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }

    
}
