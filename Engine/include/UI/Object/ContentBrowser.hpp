#pragma once

#include "imgui.h"
#include "imgui_internal.h"
#include <string>

class WindowContentBrowser{
private:
    bool once = true;
public:
    WindowContentBrowser() = default;
    void draw();
};