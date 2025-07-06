#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "WindowObjects.hpp"
#include "ImGuiRender.hpp"

#include "GState.hpp"
#include "imgui.h"
#include "FBO.hpp"
#include <iostream>



class UIManager {
public:
    UIManager() = default;
    
    ImGuiRender imGui;

    void drawUI();

private:
    
    bool m_canResize = true;
    bool m_checkDockingOnNextFrame = false;
    bool m_wasMovingWindowLastFrame = true;
    std::string m_layoutBeforeMove;

    WindowContentBrowser contentBrowser;
    WindowDetailsPanel detailsPanel;
    WindowHierarchy hierarchy;
    WindowViewPort viewPort;
    WindowTitleBar titleBar;

    void updateDockingLogic();
    bool areAllWindowsDocked();

    void drawDockspaceLayout();
};