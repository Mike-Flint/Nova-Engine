#pragma once

#include <glm/glm.hpp>
#include "AppWindow.hpp"

#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "ImGuizmo.h"


class ImGuiLayer{
public:
    ImGuiLayer();
    ~ImGuiLayer();

    void newFrame();
    void render();
};

