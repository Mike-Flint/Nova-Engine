#pragma once
#include "Renderer.hpp"
#include "imgui.h"
#include "imgui_internal.h"

class WindowViewPort{
public:
    WindowViewPort() = default;
    Renderer render;
    void draw();
};
