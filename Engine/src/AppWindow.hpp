#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Windows.h>

#include <glm/glm.hpp>
#include <iostream>

#define KEY_RELEASE                0
#define KEY_PRESS                  1
#define KEY_REPEAT                 2

#define CURSOR_NORMAL          0x00034001
#define CURSOR_HIDDEN          0x00034002
#define CURSOR_DISABLED        0x00034003
#define CURSOR_CAPTURED        0x00034004


class AppWindow{
private:
    inline static bool once = true;

    static GLFWmonitor* findCurrentMonitor();
public:
    inline static GLFWwindow *Window = nullptr;

    static void Init(const char* nameWindow ,glm::ivec2 winSize);
    static bool Update();

    static glm::ivec2 GetWindowSize();
    static void SetWindowSize(int width, int height);

    static glm::ivec2 GetWindowPos();
    static void SetWindowPos(int xpos, int ypos);

    static glm::dvec2 GetCursorPos();
    static void SetCursorPos(double xpos, double ypos);

    static void SetFullscreen();
    static void SetMaximized();

    static int GetKey(int key);
    static void SetCursorMode(int value);

    static void SetWindowShouldClose();
    static void IconifyWindow();
    static void Destroy();
};

