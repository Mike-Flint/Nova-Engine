#pragma once
#include <GLFW/glfw3.h>
#include "GState.h"
#include <glm/glm.hpp>

class WindowInput{
private:
    const int edgeThreshold = 2;
    const int minWidth = 700;
    const int minHeight = 400;

    enum class ResizeState {
        NONE,
        LEFT,
        RIGHT,
        TOP,
        BOTTOM,
        TOP_LEFT,
        TOP_RIGHT,
        BOTTOM_LEFT,
        BOTTOM_RIGHT
    };

    ResizeState m_currentResizeState = ResizeState::NONE;

    glm::ivec2 m_initialWindow;
    glm::ivec2 m_initialSize;
    glm::dvec2 m_initialMouse;
    glm::dvec2 m_initialScreenMouse;

    ResizeState getHoveredState() const;
    void updateCursor(ResizeState state);
    void applyResize();

public:
    void resizeWindow();
    void windowInput();

    static GLFWmonitor* findCurrentMonitor();
};