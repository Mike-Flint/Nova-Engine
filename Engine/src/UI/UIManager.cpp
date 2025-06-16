#include "UIManager.h"


const float TITLE_BAR_HEIGHT = 32.0f;
const float BUTTON_WIDTH = 50.0f;

// UIManager.cpp

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
    ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);


    // 2. Тепер малюємо ВСІ наші звичайні вікна
    // -----------------------------------------------------------------
    // ImGui автоматично дозволить їх "пристиковувати" до нашого Dockspace

    drawTitleBar(); // Ваш кастомний TitleBar


    // drawOutliner(viewPortSize);
    // drawDetailsPanel(viewPortSize);
    // drawContentBrowser(viewPortSize);
    // drawaViewport(vpFBO); // Viewport теж буде докабельним вікном!



    // Можна додати й інші стандартні вікна для демонстрації
    // ImGui::ShowDemoWindow(); 

    // 3. Завершуємо створення нашого головного вікна
    // -----------------------------------------------------------------
    ImGui::End();


    // 4. Ваш старий код для обробки вводу та рендерингу
    // -----------------------------------------------------------------
    // winInput.resizeWindow();
    winInput.windowInput();
    imGui.render();
}


void UIManager::drawTitleBar() {
    
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5.0f, 11.0f));
    if (ImGui::BeginMainMenuBar()) {
        
        // Створюємо перший пункт меню, наприклад, "File"
        if (ImGui::BeginMenu("File")) {
            // Додаємо підпункти
            if (ImGui::MenuItem("New Project", "Ctrl+N")) {
                // Код, який виконається при натисканні
            }
            if (ImGui::MenuItem("Open Project", "Ctrl+O")) {
                // ...
            }
            ImGui::Separator(); // Додаємо розділювач
            if (ImGui::MenuItem("Exit")) {
                glfwSetWindowShouldClose(GState::window, GLFW_TRUE);
            }
            ImGui::EndMenu(); // Завершуємо меню "File"
        }

        // Створюємо другий пункт меню, наприклад, "Edit"
        if (ImGui::BeginMenu("Edit")) {
            if (ImGui::MenuItem("Undo", "Ctrl+Z")) {}
            if (ImGui::MenuItem("Redo", "Ctrl+Y")) {}
            ImGui::EndMenu();
        }

        ImGui::PopStyleVar();

        float totalButtonsWidth = BUTTON_WIDTH * 3;
        ImGui::SetCursorPosX(ImGui::GetWindowWidth() - totalButtonsWidth - 2);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2);

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f)); 
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.5f, 0.5f, 0.5f, 0.5f)); 
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.7f, 0.7f, 0.7f, 0.5f)); 

        // Рисуем первую кнопку
        if (ImGui::Button("-##minimize", ImVec2(BUTTON_WIDTH, TITLE_BAR_HEIGHT))) {
            glfwIconifyWindow(GState::window);
        }
        ImGui::SameLine(0, 0);

        // Рисуем вторую кнопку
        if (ImGui::Button("@##maximize", ImVec2(BUTTON_WIDTH, TITLE_BAR_HEIGHT))) {
            if (GState::isWindowMaximized) {
                // ... ваш код для восстановления окна ...
                glfwSetWindowPos(GState::window, m_preMaximizePos.x, m_preMaximizePos.y);
                glfwSetWindowSize(GState::window, m_preMaximizeSize.width, m_preMaximizeSize.height);
                GState::isWindowMaximized = false;
            } else {
                // ... ваш код для максимизации окна ...
                glfwGetWindowPos(GState::window, &m_preMaximizePos.x, &m_preMaximizePos.y);
                glfwGetWindowSize(GState::window, &m_preMaximizeSize.width, &m_preMaximizeSize.height);
                GLFWmonitor* monitor = WindowInput::findCurrentMonitor();
                if (monitor) {
                    int workX, workY, workWidth, workHeight;
                    glfwGetMonitorWorkarea(monitor, &workX, &workY, &workWidth, &workHeight);
                    glfwSetWindowPos(GState::window, workX, workY);
                    glfwSetWindowSize(GState::window, workWidth, workHeight);
                    GState::isWindowMaximized  = true;
                }
            }
        }

        // Убираем стили, которые применялись к первым двум кнопкам
        ImGui::PopStyleColor(3);

        // --- БЛОК 2: Стили ТОЛЬКО для кнопки "Закрыть" ---
        ImGui::SameLine(0, 0);

        // Задаем новый, КРАСНЫЙ стиль
        // R=1.0f, G=0.2f, B=0.2f - приятный красный оттенок
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));           // Обычное состояние - прозрачное
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.2f, 0.2f, 0.8f));   // При наведении - полупрозрачный красный
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8f, 0.1f, 0.1f, 1.0f));    // При нажатии - насыщенный темно-красный

        // Рисуем красную кнопку
        if (ImGui::Button("X##close", ImVec2(BUTTON_WIDTH, TITLE_BAR_HEIGHT))) {
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

        ImGui::EndMainMenuBar();
    }
}

void UIManager::drawOutliner(const glm::ivec2& viewPortSize) {
    // ImGui::SetNextWindowPos(ImVec2((float)viewPortSize.x, TITLE_BAR_HEIGHT));
    // ImGui::SetNextWindowSize(ImVec2((float)GState::winSize.width - viewPortSize.x, (float)viewPortSize.y / 2));
    ImGui::Begin("Outliner");
    ImGui::Text("Give ME");
    ImGui::End();
}

void UIManager::drawDetailsPanel(const glm::ivec2& viewPortSize) {
    // ImGui::SetNextWindowPos(ImVec2((float)viewPortSize.x, (float)viewPortSize.y / 2 + TITLE_BAR_HEIGHT));
    // ImGui::SetNextWindowSize(ImVec2((float)GState::winSize.width - viewPortSize.x, (float)viewPortSize.y / 2));
    ImGui::Begin("Details");
    ImGui::Text("Details");
    ImGui::End();
}

void UIManager::drawContentBrowser(const glm::ivec2& viewPortSize) {
    // ImGui::SetNextWindowPos(ImVec2(0, (float)viewPortSize.y + TITLE_BAR_HEIGHT));
    // ImGui::SetNextWindowSize(ImVec2((float)GState::winSize.width, (float)GState::winSize.height - viewPortSize.y - TITLE_BAR_HEIGHT));
    ImGui::Begin("Content Browser");
    ImGui::Text("File");
    ImGui::End();
}

void UIManager::drawaViewport( FBO* vpFBO) {
    // ImGui::SetNextWindowPos(ImVec2(0, 500));
    // ImGui::SetNextWindowSize(ImVec2(500, 500));

    ImGui::Begin("3D Viewport");

    // Отримуємо розмір доступної області всередині вікна
    ImVec2 viewportSize = ImGui::GetContentRegionAvail();

    // Малюємо текстуру з нашого FBO
    // (void*)(intptr_t)myFBO.textureId - стандартний спосіб передати OpenGL ID в ImGui
    // ImVec2(0, 1) і ImVec2(1, 0) перевертають текстуру по осі Y, що часто потрібно для OpenGL
    ImGui::Image((void*)(intptr_t)vpFBO->textureId, viewportSize, ImVec2(0, 1), ImVec2(1, 0));

    ImGui::End();
}