#pragma once
#include <windows.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include "GState.hpp"
#include "imgui.h"
// #include "imgui_impl_opengl3_loader.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"
#include "imstb_rectpack.h"
#include "imstb_textedit.h"
#include "imstb_truetype.h"

class ImGuiRender{
    public:
        ImGuiRender(){
            init();
        };

        void init();
        void newFrame();
        void render();
        void destroy();
};

int GetTaskbarHeight();

