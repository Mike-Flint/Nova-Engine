#include "UIManager.hpp"


const float BUTTON_HEIGHT = 32.0f;
const float BUTTON_WIDTH = 50.0f;

void setupInitialDockSpaceLayout() {
    ImGuiID dockspace_id = ImGui::GetID("DockSpace"); // ID головного DockSpace

    // Очистити будь-який попередній макет (важливо для запуску)
    ImGui::DockBuilderRemoveNode(dockspace_id);
    // Додати головний вузол док-простору назад
    ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);
    // Встановити розмір головного вузла DockSpace, щоб він заповнював весь батьківський простір
    ImGui::DockBuilderSetNodeSize(dockspace_id, ImVec2(ImGui::GetMainViewport()->WorkSize.x, 35));


    ImGuiID top_node_id;      // ID вузла для верхньої панелі
    ImGuiID central_node_id;  // ID вузла для решти панелей

    // Розділити головний DockSpace:
    // ImGuiDir_Up: створює вузол зверху
    // MENU_BAR_HEIGHT / ImGui::GetMainViewport()->WorkSize.y: пропорція висоти верхнього вузла
    // &top_node_id: ID нового верхнього вузла
    // ¢ral_node_id: ID решти (центрального) вузла
    ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Up, 35 / ImGui::GetMainViewport()->WorkSize.y, &top_node_id, &central_node_id);

    // *** Прикріплюємо нашу панель "CustomTitleBar" до верхнього вузла ***
    ImGui::DockBuilderDockWindow("CustomTitleBar", top_node_id);

    // === Встановлюємо прапорці для верхнього вузла, щоб зробити його ФІКСОВАНИМ ===
    ImGuiDockNode* topNode = ImGui::DockBuilderGetNode(top_node_id);
    if (topNode) {
        topNode->LocalFlags |= ImGuiDockNodeFlags_NoTabBar |         // Не показувати панель вкладок (бо там тільки 1 вікно)
                                ImGuiDockNodeFlags_NoResize |       // Не дозволяти змінювати розмір цього вузла користувачем
                                ImGuiDockNodeFlags_NoDocking |      // Не дозволяти іншим вікнам докуватися *у* цей вузол
                                ImGuiDockNodeFlags_NoDockingOverMe; // Не дозволяти цей вузол перетягувати кудись
    }

    // *** Прикріплюємо інші панелі до центрального вузла ***
    // (ВАЖЛИВО: "My Content Panel" має бути точним ім'ям ImGui-вікна,
    // яке використовується у функції drawUI()!)
    ImGui::DockBuilderDockWindow("Content Panel", central_node_id);

    // Завершуємо створення макета DockSpace
    ImGui::DockBuilderFinish(dockspace_id);
}


void UIManager::drawUI() {
    // Встановлюємо, щоб наше вікно займало весь екран
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);

    // Встановлюємо стилі, щоб прибрати все зайве: рамки, відступи, фон
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    // Прапорці для нашого фонового вікна
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar | 
                                    ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus | 
                                    ImGuiWindowFlags_NoNavFocus;
    
    // Починаємо створення невидимого вікна
    ImGui::Begin("DockSpace_Window", nullptr, window_flags);

    // Повертаємо стилі до стандартних значень
    ImGui::PopStyleVar(3);

    // Створюємо сам Dockspace всередині цього вікна
    ImGuiID dockspace_id = ImGui::GetID("DockSpace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);


    {
        ImVec2 window_pos = ImGui::GetWindowPos();
        ImVec2 window_size = ImGui::GetWindowSize();

        glfwSetWindowPos(GState::window, window_pos.x, window_pos.y);
        glfwSetWindowSize(GState::window, window_size.x, window_size.y);
    }
    drawTitleBar();

    int dockSpaceInitialized = false;
    if(!dockSpaceInitialized){
        dockSpaceInitialized = true;
        setupInitialDockSpaceLayout();
    }

   

    
    // contentBrowser.draw();
    // detailsPanel.draw();
    // outliner.draw();
    viewPort.draw();
    

    ImGui::ShowDemoWindow();

    ImGui::End();
    
    // winInput.resizeWindow();
    winInput.windowInput();
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

void UIManager::drawTitleBar() {
    
    
    if (ImGui::Begin("CustomTitleBar", nullptr, ImGuiWindowFlags_NoDocking)) {
        
        if (ImGui::Button("File")) {
            ImGui::OpenPopup("FileMenuPopup");
        }
        if (ImGui::BeginPopup("FileMenuPopup")) {
            if (ImGui::MenuItem("New Project", "Ctrl+N")) {
                // Your logic
            }
            if (ImGui::MenuItem("Open Project", "Ctrl+O")) {
                // Your logic
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Exit")) {
                glfwSetWindowShouldClose(GState::window, GLFW_TRUE);
            }
            ImGui::EndPopup();
        }
        ImGui::SameLine();

        // Edit Menu
        if (ImGui::Button("Edit")) {
            ImGui::OpenPopup("EditMenuPopup");
        }
        if (ImGui::BeginPopup("EditMenuPopup")) {
            if (ImGui::MenuItem("Undo", "Ctrl+Z")) {}
            if (ImGui::MenuItem("Redo", "Ctrl+Y")) {}
            ImGui::EndPopup();
        }
        ImGui::SameLine();

        // Pop style after menu items, before window control buttons

        // --- 2. Window Control Buttons (Minimize, Maximize, Close) ---
        float totalButtonsWidth = BUTTON_WIDTH * 3;
        // Position cursor to the right side of the current *ImGui window*
        ImGui::SetCursorPosX(ImGui::GetWindowWidth() - totalButtonsWidth - ImGui::GetStyle().WindowPadding.x - ImGui::GetStyle().ItemSpacing.x * 2);
        // ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2); // Adjust Y if needed, SameLine often handles it

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f)); // Transparent
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.5f, 0.5f, 0.5f, 0.5f)); // Grey hover
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.7f, 0.7f, 0.7f, 0.5f)); // Darker grey active

        if (ImGui::Button("-##minimize", ImVec2(BUTTON_WIDTH, BUTTON_HEIGHT))) {
            glfwIconifyWindow(GState::window);
        }
        ImGui::SameLine(0, 0); // No spacing between buttons

        if (ImGui::Button("@##maximize", ImVec2(BUTTON_WIDTH, BUTTON_HEIGHT))) {
            if (GState::isWindowMaximized) {
                // Restore window
                glfwSetWindowPos(GState::window, m_preMaximizePos.x, m_preMaximizePos.y);
                glfwSetWindowSize(GState::window, m_preMaximizeSize.width, m_preMaximizeSize.height);
                GState::isWindowMaximized = false;
            } else {
                // Maximize window
                glfwGetWindowPos(GState::window, &m_preMaximizePos.x, &m_preMaximizePos.y);
                glfwGetWindowSize(GState::window, &m_preMaximizeSize.width, &m_preMaximizeSize.height);
                GLFWmonitor* monitor = WindowInput::findCurrentMonitor();
                if (monitor) {
                    int workX, workY, workWidth, workHeight;
                    glfwGetMonitorWorkarea(monitor, &workX, &workY, &workWidth, &workHeight);
                    glfwSetWindowPos(GState::window, workX, workY);
                    glfwSetWindowSize(GState::window, workWidth, workHeight);
                    GState::isWindowMaximized = true;
                }
            }
        }
        ImGui::PopStyleColor(3);

        ImGui::SameLine(0, 0); // No spacing between buttons

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));           // Transparent
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.2f, 0.2f, 0.8f));   // Red hover
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8f, 0.1f, 0.1f, 1.0f));    // Dark red active

        if (ImGui::Button("X##close", ImVec2(BUTTON_WIDTH, BUTTON_HEIGHT))) {
            glfwSetWindowShouldClose(GState::window, GLFW_TRUE);
        }
        ImGui::PopStyleColor(3);

        if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !GState::isMouseBusy) {
            if (GState::mouse.x < (GState::winSize.width - totalButtonsWidth)) {
                m_isDraggingWindow = true;
                GState::isMouseBusy = true;
                if(GState::isWindowMaximized){
                    GState::isWindowMaximized = false;

                    float mouseRatioX = (float)(GState::mouse.x / GState::winSize.width);

                    glfwSetWindowPos(GState::window, (GState::winPos.x + (int)GState::mouse.x) - (int)(mouseRatioX * m_preMaximizeSize.width), GState::winPos.y);
                    glfwSetWindowSize(GState::window, m_preMaximizeSize.width, m_preMaximizeSize.height);
                    GState::update();
                }
                m_dragOffset = glm::ivec2(GState::mouse.x, GState::mouse.y);
            }
        }
        
        if (m_isDraggingWindow) {
            if (ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
                int newWinPosX = GState::winPos.x + (int)GState::mouse.x - m_dragOffset.x;
                int newWinPosY = GState::winPos.y + (int)GState::mouse.y - m_dragOffset.y;
                glfwSetWindowPos(GState::window, newWinPosX, newWinPosY);
            }
        }


        if (ImGui::IsMouseReleased(ImGuiMouseButton_Left) && m_isDraggingWindow) {
            m_isDraggingWindow = false;
            GState::isMouseBusy = false; 
        }

        ImGui::End();
    }

}