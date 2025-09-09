#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "WindowObjects.hpp"
#include "AppWindow.hpp"
#include "ImGuiLayer.hpp"
#include "ToolsUI.hpp"
#include "Scene.hpp"


class UIManager {
public:
    UIManager(): 
        contentBrowser(m_State, &GScene),
        detailsPanel(m_State, &GScene),
        hierarchy(m_State, &GScene),
        viewPort(m_State, &GScene),
        titleBar(m_State){}

    void drawUI();

private:

    bool m_CheckDockingOnNextFrame = false;
    bool m_WasMovingWindowLastFrame = true;
    std::string m_LayoutBeforeMove;

    glm::ivec2 m_SizeWindow;

    EditorWindows m_State;
    ImGuiLayer imGui;

    Scene GScene;

    ImWindows::ContentBrowser contentBrowser;
    ImWindows::DetailsPanel detailsPanel;
    ImWindows::Hierarchy hierarchy;
    ImWindows::ViewPort viewPort;
    ImWindows::TitleBar titleBar;

    void updateDockingLogic();
    void syncWindowWithImGui();
    void drawDockspaceLayout();
    bool areAllWindowsDocked();
};