#pragma once
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

struct GState {
public:
    static GLFWwindow *window;

    static glm::dvec2 mouse;
    static glm::ivec2 winPos;
    static glm::ivec2 winSize;
    static glm::ivec2 vpPerc;
    static glm::ivec2 vpSize;


    static bool isMouseBusy;
    static bool isWindowMaximized;

    static void update();
};

