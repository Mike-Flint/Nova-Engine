#include "TitleBar.hpp"

const float BUTTON_HEIGHT = 32.0f;
const float BUTTON_WIDTH = 50.0f;

GLFWmonitor* WindowTitleBar::findCurrentMonitor() {
    int monitorCount;
    GLFWmonitor** monitors = glfwGetMonitors(&monitorCount);

    if (monitorCount == 0) {
        return nullptr;
    }

    // Позиція та розмір вікна (з вашого глобального стану)
    int winX = GState::winPos.x;
    int winY = GState::winPos.y;
    int winWidth = GState::winSize.width;
    int winHeight = GState::winSize.height;

    GLFWmonitor* bestMonitor = nullptr;
    long long maxOverlap = 0; // Використовуємо long long, щоб уникнути переповнення

    for (int i = 0; i < monitorCount; i++) {
        GLFWmonitor* monitor = monitors[i];
        
        // Отримуємо робочу область монітора (враховує панель завдань)
        int monX, monY, monWidth, monHeight;
        glfwGetMonitorWorkarea(monitor, &monX, &monY, &monWidth, &monHeight);

        // Обчислюємо площу перетину
        int overlapX = glm::max(winX, monX);
        int overlapY = glm::max(winY, monY);

        int overlapRight = glm::min(winX + winWidth, monX + monWidth);
        int overlapBottom = glm::min(winY + winHeight, monY + monHeight);

        long long overlapWidth = overlapRight - overlapX;
        long long overlapHeight = overlapBottom - overlapY;
        
        // Якщо є перетин (ширина та висота > 0)
        if (overlapWidth > 0 && overlapHeight > 0) {
            long long overlapArea = overlapWidth * overlapHeight;
            
            // Якщо цей монітор має більшу площу перетину, він стає найкращим кандидатом
            if (overlapArea > maxOverlap) {
                maxOverlap = overlapArea;
                bestMonitor = monitor;
            }
        }
    }
    
    // Якщо вікно повністю за межами всіх моніторів, повернемо основний монітор
    if (bestMonitor == nullptr) {
        return glfwGetPrimaryMonitor();
    }

    return bestMonitor;
}

StateTitleBar* WindowTitleBar::Draw(){
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(ImGui::GetStyle().FramePadding.x, 10.0f));
    if (ImGui::BeginMenuBar()) {

        if (ImGui::BeginMenu("File")) {
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
            ImGui::EndMenu();
        }
    
        if (ImGui::BeginMenu("Edit")) {
            if (ImGui::MenuItem("Undo", "Ctrl+Z")) {}
            if (ImGui::MenuItem("Redo", "Ctrl+Y")) {}
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Windows")) {
            if (ImGui::MenuItem("ContentBrowser", (barState.ShowContentBrowser)? "Close" : "Open")) 
                barState.ShowContentBrowser = !barState.ShowContentBrowser;
            if (ImGui::MenuItem("DetailsPanel", (barState.ShowDetailsPanel)? "Close" : "Open")) 
                barState.ShowDetailsPanel = !barState.ShowDetailsPanel;
            if (ImGui::MenuItem("Hierarchy", (barState.ShowHierarchy)? "Close" : "Open")) 
                barState.ShowHierarchy = !barState.ShowHierarchy;
            
            ImGui::EndMenu();
        }

        
        ImGui::PopStyleVar();
        // Pop style after menu items, before window control buttons

        // --- 2. Window Control Buttons (Minimize, Maximize, Close) ---
        float totalButtonsWidth = BUTTON_WIDTH * 3;
        // Position cursor to the right side of the current *ImGui window*
        ImGui::SetCursorPosX(ImGui::GetWindowWidth() - totalButtonsWidth);
        // ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2); // Adjust Y if needed, SameLine often handles it

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f)); // Transparent
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.5f, 0.5f, 0.5f, 0.5f)); // Grey hover
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.7f, 0.7f, 0.7f, 0.5f)); // Darker grey active

        if (ImGui::Button("-##minimize", ImVec2(BUTTON_WIDTH, BUTTON_HEIGHT))) {
            glfwIconifyWindow(GState::window);
        }
        ImGui::SameLine(0, 0); // No spacing between buttons

        if (ImGui::Button("@##maximize", ImVec2(BUTTON_WIDTH, BUTTON_HEIGHT))) {
            if (barState.CanResize) {
                // Maximize window
                glfwGetWindowPos(GState::window, &m_preMaximizePos.x, &m_preMaximizePos.y);
                glfwGetWindowSize(GState::window, &m_preMaximizeSize.width, &m_preMaximizeSize.height);
                GLFWmonitor* monitor = findCurrentMonitor();
                if (monitor) {
                    int workX, workY, workWidth, workHeight;
                    glfwGetMonitorWorkarea(monitor, &workX, &workY, &workWidth, &workHeight);
                    glfwSetWindowPos(GState::window, workX, workY);
                    glfwSetWindowSize(GState::window, workWidth, workHeight);
                    
                }
                barState.CanResize = false;
            } else {
                // Restore window
                glfwSetWindowPos(GState::window, m_preMaximizePos.x, m_preMaximizePos.y);
                glfwSetWindowSize(GState::window, m_preMaximizeSize.width, m_preMaximizeSize.height);
                barState.CanResize = true;
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

        if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !ImGui::IsAnyItemHovered()) {
            m_isDraggingWindow = true;
            if(!barState.CanResize){
                barState.CanResize = true;

                float mouseRatioX = (float)(GState::mouse.x / GState::winSize.width);

                glfwSetWindowPos(GState::window, (GState::winPos.x + (int)GState::mouse.x) - (int)(mouseRatioX * m_preMaximizeSize.width), GState::winPos.y);
                glfwSetWindowSize(GState::window, m_preMaximizeSize.width, m_preMaximizeSize.height);
                GState::update();
            }
            m_dragOffset = glm::ivec2(GState::mouse.x, GState::mouse.y);
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
        }

        ImGui::EndMenuBar();
        
    }
    return &barState;
}

