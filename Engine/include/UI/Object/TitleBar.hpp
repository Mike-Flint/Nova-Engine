#pragma once

#include <GLFW/glfw3.h>
#include "imgui.h"
#include "GState.hpp"


struct StateTitleBar{
public: 
    bool ShowContentBrowser = false;
    bool ShowDetailsPanel = false;
    bool ShowHierarchy = false;

    bool CanResize = true;
};

class WindowTitleBar{
public:
    StateTitleBar barState;
    WindowTitleBar() = default;
    StateTitleBar* Draw();

private:
    GLFWmonitor *findCurrentMonitor();

    glm::ivec2 m_preMaximizePos;
    glm::ivec2 m_preMaximizeSize;

    glm::ivec2 m_dragOffset{0, 0};

    bool m_isDraggingWindow = false;
};
