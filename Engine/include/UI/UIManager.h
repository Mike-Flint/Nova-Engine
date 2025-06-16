#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "WindowObjects.h"
#include "WindowInput.h"
#include "ImGuiRender.h"

#include "GState.h"
#include "imgui.h"
#include "FBO.h"
#include <iostream>



class UIManager {
public:
    UIManager() = default;
    ImGuiRender imGui;

    void drawUI(const glm::ivec2& viewPortSize, FBO* vpFBO = nullptr);

private:
    WindowInput winInput;

    glm::ivec2 m_preMaximizePos;
    glm::ivec2 m_preMaximizeSize;

    glm::ivec2 m_dragOffset{0, 0};

    bool m_isDraggingWindow = false;

    void drawTitleBar();
    void drawOutliner(const glm::ivec2& viewPortSize);
    void drawDetailsPanel(const glm::ivec2& viewPortSize);
    void drawContentBrowser(const glm::ivec2& viewPortSize);
    void drawaViewport( FBO* vpFBO);


};