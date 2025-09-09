#include "TitleBar.hpp"

const float BUTTON_HEIGHT = 32.0f;
const float BUTTON_WIDTH = 50.0f;

void ImWindows::TitleBar::maximizeOrRestoreWindow(){
    if (m_State->CanResize) {
        // Maximize window

        m_preMaximizePos = AppWindow::GetWindowPos();
        m_preMaximizeSize = AppWindow::GetWindowSize();

        AppWindow::SetMaximized();

        // AppWindow::SetFullscreen();

        m_State->CanResize = false;
    } else {
        // Restore window
        AppWindow::SetWindowPos(m_preMaximizePos.x, m_preMaximizePos.y);
        AppWindow::SetWindowSize(m_preMaximizeSize.width, m_preMaximizeSize.height);
        m_State->CanResize = true;
    }
}

void ImWindows::TitleBar::drawWindowButtons(){
    static ImGui::FileBrowser fileDialog;

    if (ImGui::BeginMenu("File")) {
        if (ImGui::MenuItem("New Project", "Ctrl+N")) {
            fileDialog.Open();
        }
        if (ImGui::MenuItem("Open Project", "Ctrl+O")) {
            fileDialog.Open();
        }
        ImGui::Separator();
        if (ImGui::MenuItem("Exit")) {
            AppWindow::SetWindowShouldClose();
        }
        ImGui::EndMenu();
    }

    fileDialog.SetTypeFilters({ ".png", ".jpg", ".fbx" });
    fileDialog.Display();

    if (ImGui::BeginMenu("Edit")) {
        if (ImGui::MenuItem("Undo", "Ctrl+Z")) {}
        if (ImGui::MenuItem("Redo", "Ctrl+Y")) {}
        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Windows")) {
        if (ImGui::MenuItem("ContentBrowser", (m_State->ContentBrowser.Show)? "Close" : "Open")){
            EditorWindowConfig& winConfig = m_State->ContentBrowser;
            if (winConfig.Show){
                winConfig.Dock = true;
                winConfig.Show = false;
                winConfig.NeedsSettingsDock = true;
            }else{  
                winConfig.Show = true;
            }
        }

        if (ImGui::MenuItem("DetailsPanel", (m_State->DetailsPanel.Show)? "Close" : "Open")){
            EditorWindowConfig& winConfig = m_State->DetailsPanel;
            if (winConfig.Show){
                winConfig.Dock = true;
                winConfig.Show = false;
                winConfig.NeedsSettingsDock = true;
            }else{  
                winConfig.Show = true;
            }
        }

        if (ImGui::MenuItem("Hierarchy", (m_State->Hierarchy.Show)? "Close" : "Open")){
            EditorWindowConfig& winConfig = m_State->Hierarchy;
            if (winConfig.Show){
                winConfig.Dock = true;
                winConfig.Show = false;
                winConfig.NeedsSettingsDock = true;
            }else{  
                winConfig.Show = true;
            }
        }
    
        ImGui::EndMenu();
    }


    float totalButtonsWidth = BUTTON_WIDTH * 3;

    ImGui::SetCursorPosX(ImGui::GetWindowWidth() - totalButtonsWidth);


    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f)); 
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.5f, 0.5f, 0.5f, 0.5f)); 
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.7f, 0.7f, 0.7f, 0.5f));

    if (ImGui::Button("-##minimize", ImVec2(BUTTON_WIDTH, BUTTON_HEIGHT))) {
        AppWindow::IconifyWindow();
    }
    ImGui::SameLine(0, 0);

    static int texWidth, texHeight;

    static ImTextureID maxImage = Model::TextureFromFile("layers.png", "D:/Documents/Dev/C++/Nova-Engine/Assets/Textures", &texWidth, &texHeight);

if (ImGui::Button("@##maximize", ImVec2(BUTTON_WIDTH, BUTTON_HEIGHT))) {
    maximizeOrRestoreWindow();
}

    ImGui::PopStyleColor(3);

    ImGui::SameLine(0, 0);

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.2f, 0.2f, 0.8f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8f, 0.1f, 0.1f, 1.0f));

    if (ImGui::Button("X##close", ImVec2(BUTTON_WIDTH, BUTTON_HEIGHT))) {
        AppWindow::SetWindowShouldClose();
    }
    ImGui::PopStyleColor(3);
}

void ImWindows::TitleBar::handleWindowDrag(){
    if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !ImGui::IsAnyItemHovered()) {

        m_isDraggingWindow = true;
        ImVec2 cursorPos = ImGui::GetMousePos();
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        
        if(!m_State->CanResize){
            m_State->CanResize = true;

            AppWindow::SetWindowSize(m_preMaximizeSize.width, m_preMaximizeSize.height);
    
            float mouseRatioX = (float)((cursorPos.x - viewport->WorkPos.x) / viewport->WorkSize.x);
            m_dragOffset = glm::ivec2(
                    mouseRatioX * m_preMaximizeSize.width,
                    0
            );
        }else{
            m_dragOffset = glm::ivec2( 
                cursorPos.x - viewport->WorkPos.x ,
                cursorPos.y - viewport->WorkPos.y );
        }
        
    }

    if (m_isDraggingWindow){
        if (ImGui::IsMouseDown(ImGuiMouseButton_Left)){
            ImVec2 cursorPos = ImGui::GetMousePos();

            int newWinPosX = (int)(cursorPos.x - m_dragOffset.x);
            int newWinPosY = (int)(cursorPos.y - m_dragOffset.y);
            AppWindow::SetWindowPos(newWinPosX, newWinPosY);
        }
    }

    if (ImGui::IsMouseReleased(ImGuiMouseButton_Left) && m_isDraggingWindow) {
        m_isDraggingWindow = false;
    }
}

void ImWindows::TitleBar::handleShortcuts(){
    if (ImGui::IsKeyPressed(ImGuiKey_Escape)) {
        AppWindow::SetWindowShouldClose();
    }
    if (ImGui::IsKeyPressed(ImGuiKey_F11)) {
        maximizeOrRestoreWindow();
    }
}

void ImWindows::TitleBar::Draw(){
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(ImGui::GetStyle().FramePadding.x, 10.0f));
    if (ImGui::BeginMenuBar()) {
        ImGui::PopStyleVar();
        
        drawWindowButtons();

        handleWindowDrag();

        handleShortcuts();

        ImGui::EndMenuBar();
    }
}

