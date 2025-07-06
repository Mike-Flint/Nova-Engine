#pragma once

#include "imgui.h"
#include "ImGuiRender.hpp"

class WindowDetailsPanel{
private:
    bool once = true;
public:
    WindowDetailsPanel() = default;
    void draw();
};