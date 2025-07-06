#pragma once

#include "imgui.h"
#include "imgui_internal.h"

class WindowHierarchy{
private:
    bool once = true;
public:
    WindowHierarchy() = default;
    void draw();
};