#include "UIManager.hpp"

bool UIManager::areAllWindowsDocked(){
    ImGuiContext& g = *GImGui;

    // Перебираем все окна, которые знает ImGui
    for (ImGuiWindow* window : g.Windows){

        if (!window->WasActive)
            continue;

        if (window->Flags & ImGuiWindowFlags_ChildWindow)
            continue;

        if (window->Flags & ImGuiWindowFlags_NoDocking)
            continue;

        if (window->DockNode == nullptr){
            return false; 
        }
    }

    return true;
}

void UIManager::updateDockingLogic() {
    ImGuiContext& g = *GImGui;
    bool isMovingWindowNow = (g.MovingWindow != nullptr);


    if (m_checkDockingOnNextFrame){
        m_checkDockingOnNextFrame = false;
        if (!areAllWindowsDocked()){

            if (!m_layoutBeforeMove.empty()){
                ImGui::LoadIniSettingsFromMemory(m_layoutBeforeMove.c_str(), m_layoutBeforeMove.size());
            }
        }
        m_layoutBeforeMove.clear();
    }

    if (!m_wasMovingWindowLastFrame && isMovingWindowNow){
        size_t data_size = 0;
        const char* layout_data = ImGui::SaveIniSettingsToMemory(&data_size);
        m_layoutBeforeMove.assign(layout_data, data_size);
    }

    if (m_wasMovingWindowLastFrame && !isMovingWindowNow)
        m_checkDockingOnNextFrame = true;
    
    m_wasMovingWindowLastFrame = isMovingWindowNow;
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
    
    if (!m_canResize){
        window_flags |= ImGuiWindowFlags_NoResize;
    }

    ImGui::Begin("DockSpace_Window", nullptr, window_flags);

    ImGui::PopStyleVar(4);

    // Створюємо сам Dockspace всередині цього вікна
    ImGuiID dockspace_id = ImGui::GetID("DockSpace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);
}

void UIManager::drawUI() {
    drawDockspaceLayout();

    {
        ImVec2 window_pos = ImGui::GetWindowPos();
        ImVec2 window_size = ImGui::GetWindowSize();

        glfwSetWindowPos(GState::window, (int)window_pos.x, (int)window_pos.y);
        glfwSetWindowSize(GState::window, (int)window_size.x, (int)window_size.y);
    }

    StateTitleBar* barState = titleBar.Draw();

    if(barState->ShowContentBrowser){
        contentBrowser.draw();
    }
    if(barState->ShowDetailsPanel){
        detailsPanel.draw();
    }
    if(barState->ShowHierarchy){
        hierarchy.draw();
    }
    m_canResize = barState->CanResize;


    viewPort.draw();

    ImGui::End();

    updateDockingLogic();

    if (glfwGetKey(GState::window, GLFW_KEY_ESCAPE) == GLFW_PRESS) 
        glfwSetWindowShouldClose(GState::window, true);

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
