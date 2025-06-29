#pragma once
#include <GLFW/glfw3.h>
#include "GState.hpp"
#include <glm/glm.hpp>
#include "imgui.h"

class WindowInput{
private:
    const int edgeThreshold = 2;
    const int minWidth = 700;
    const int minHeight = 400;

    ImGuiMouseCursor_ m_currentResizeState = ImGuiMouseCursor_::ImGuiMouseCursor_None;

    glm::ivec2 m_initialWindow;
    glm::ivec2 m_initialSize;
    glm::dvec2 m_initialMouse;
    glm::dvec2 m_initialScreenMouse;

    ImGuiMouseCursor_ getHoveredState() const;
    void updateCursor(ImGuiMouseCursor_ state);
    void applyResize();

public:
    void resizeWindow();
    void windowInput();

    static GLFWmonitor* findCurrentMonitor();
};