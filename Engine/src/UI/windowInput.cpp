
#include "WindowInput.h"


void WindowInput::resizeWindow() {
    if (!GState::isWindowMaximized){
        bool isLeftMousePressed = (glfwGetMouseButton(GState::window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS);

        if (m_currentResizeState != ResizeState::NONE) {
            if (isLeftMousePressed) {
                applyResize();
            } else {

                m_currentResizeState = ResizeState::NONE;
                GState::isMouseBusy = false; 
            }
        } 
        else {
            if (!GState::isMouseBusy) {
                ResizeState hoveredState = getHoveredState();
                updateCursor(hoveredState);

                if (isLeftMousePressed && hoveredState != ResizeState::NONE) {
                    m_currentResizeState = hoveredState;
                    GState::isMouseBusy = true;

                    m_initialWindow = glm::ivec2(GState::winPos.x,  GState::winPos.y);
                    m_initialSize = glm::ivec2(GState::winSize.width, GState::winSize.height);
                    m_initialMouse = glm::dvec2(GState::mouse.x, GState::mouse.y);
                    m_initialScreenMouse = glm::dvec2((float)GState::winPos.x + GState::mouse.x, (float)GState::winPos.y + GState::mouse.y);
                }
            } else {
                updateCursor(ResizeState::NONE);
            }
        }
    }
}

WindowInput::ResizeState WindowInput::getHoveredState() const {
    bool nearLeft = GState::mouse.x >= 0 && GState::mouse.x <= edgeThreshold;
    bool nearRight = GState::mouse.x >= GState::winSize.width - edgeThreshold && GState::mouse.x <= GState::winSize.width;
    bool nearTop = GState::mouse.y >= 0 && GState::mouse.y <= edgeThreshold;
    bool nearBottom = GState::mouse.y >= GState::winSize.height - edgeThreshold && GState::mouse.y <= GState::winSize.height;

    if (nearTop && nearLeft) return ResizeState::TOP_LEFT;
    if (nearTop && nearRight) return ResizeState::TOP_RIGHT;
    if (nearBottom && nearLeft) return ResizeState::BOTTOM_LEFT;
    if (nearBottom && nearRight) return ResizeState::BOTTOM_RIGHT;
    if (nearLeft) return ResizeState::LEFT;
    if (nearRight) return ResizeState::RIGHT;
    if (nearTop) return ResizeState::TOP;
    if (nearBottom) return ResizeState::BOTTOM;

    return ResizeState::NONE;
}

void WindowInput::updateCursor(ResizeState state) {
    static GLFWcursor* cursor = nullptr;
    static ResizeState currentCursorState = ResizeState::NONE;

    if (state == currentCursorState) {
        return;
    }

    currentCursorState = state;
    if (cursor) {
        glfwDestroyCursor(cursor);
    }

    int cursorShape = GLFW_ARROW_CURSOR;
    switch (state) {
        case ResizeState::LEFT:
        case ResizeState::RIGHT:
            cursorShape = GLFW_HRESIZE_CURSOR;
            break;
        case ResizeState::TOP:
        case ResizeState::BOTTOM:
            cursorShape = GLFW_VRESIZE_CURSOR;
            break;
        case ResizeState::TOP_LEFT:
        case ResizeState::BOTTOM_RIGHT:
            cursorShape = GLFW_RESIZE_NWSE_CURSOR;
            break;
        case ResizeState::TOP_RIGHT:
        case ResizeState::BOTTOM_LEFT:
            cursorShape = GLFW_RESIZE_NESW_CURSOR;
            break;
        default:
            cursorShape = GLFW_ARROW_CURSOR;
            break;
    }

    cursor = glfwCreateStandardCursor(cursorShape);
    glfwSetCursor(GState::window, cursor);
}

void WindowInput::applyResize() {
    double dx = GState::mouse.x + (float)GState::winPos.x - (float)m_initialScreenMouse.x;
    double dy = GState::mouse.y + (float)GState::winPos.y - (float)m_initialScreenMouse.y;

    int newX = m_initialWindow.x;
    int newY = m_initialWindow.y;
    int newWidth = m_initialSize.width;
    int newHeight = m_initialSize.height;

    if (m_currentResizeState == ResizeState::LEFT || m_currentResizeState == ResizeState::TOP_LEFT || m_currentResizeState == ResizeState::BOTTOM_LEFT) {
        newX = m_initialWindow.x + (int)dx;
        newWidth = m_initialSize.width - (int)dx;
    }
    if (m_currentResizeState == ResizeState::RIGHT || m_currentResizeState == ResizeState::TOP_RIGHT || m_currentResizeState == ResizeState::BOTTOM_RIGHT) {
        newWidth = m_initialSize.width + (int)dx;
    }

    if (m_currentResizeState == ResizeState::TOP || m_currentResizeState == ResizeState::TOP_LEFT || m_currentResizeState == ResizeState::TOP_RIGHT) {
        newY = m_initialWindow.y + (int)dy;
        newHeight = m_initialSize.height - (int)dy;
    }
    if (m_currentResizeState == ResizeState::BOTTOM || m_currentResizeState == ResizeState::BOTTOM_LEFT || m_currentResizeState == ResizeState::BOTTOM_RIGHT) {
        newHeight = m_initialSize.height + (int)dy;
    }
    if (newWidth < minWidth) {
        if (m_currentResizeState == ResizeState::LEFT || m_currentResizeState == ResizeState::TOP_LEFT || m_currentResizeState == ResizeState::BOTTOM_LEFT) {
            newX = m_initialWindow.x + m_initialSize.width - minWidth;
        }
        newWidth = minWidth;
    }
    if (newHeight < minHeight) {
        if (m_currentResizeState == ResizeState::TOP || m_currentResizeState == ResizeState::TOP_LEFT || m_currentResizeState == ResizeState::TOP_RIGHT) {
            newY = m_initialWindow.y + m_initialSize.height - minHeight;
        }
        newHeight = minHeight;
    }

    if (newX != GState::winPos.x || newY != GState::winPos.y || newWidth != GState::winSize.width || newHeight != GState::winSize.height) {
        glfwSetWindowPos(GState::window, newX, newY);
        glfwSetWindowSize(GState::window, newWidth, newHeight);
    }
}

GLFWmonitor* WindowInput::findCurrentMonitor() {
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

void WindowInput::windowInput() {
    if (glfwGetKey(GState::window, GLFW_KEY_ESCAPE) == GLFW_PRESS) 
        glfwSetWindowShouldClose(GState::window, true);
}

