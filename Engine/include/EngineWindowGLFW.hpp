#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>

#include "GState.hpp"

class EngineWindowGLFW{
public:
    EngineWindowGLFW(const char* nameWindow ,glm::ivec2 winSize);
    GLFWwindow *Window;
    bool Update();
    ~EngineWindowGLFW();
};