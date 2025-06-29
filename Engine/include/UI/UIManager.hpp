#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "WindowObjects.hpp"
#include "WindowInput.hpp"
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
    WindowInput winInput;
    
    WindowContentBrowser contentBrowser;
    WindowDetailsPanel detailsPanel;
    WindowOutliner outliner;
    WindowViewPort viewPort;

    glm::ivec2 m_preMaximizePos;
    glm::ivec2 m_preMaximizeSize;

    glm::ivec2 m_dragOffset{0, 0};

    bool m_isDraggingWindow = false;

    void drawTitleBar();
};