#pragma once

#include <string>

#include <imgui.h>
#include "imgui_internal.h"

#include "AppWindow.hpp"
#include "EditorWindowState.hpp"
#include "ToolsUI.hpp"
#include "Scene.hpp"


class BaseWindow {
protected:
    Scene* GScene = nullptr;

    bool m_prevDocked = false;

    EditorWindowConfig& m_WindowToggle;

    std::string_view m_windowName;

<<<<<<< HEAD
    virtual void DrawContent() = 0;
=======
    virtual void DrawContent() = 0; 
>>>>>>> 209b5a9 (END)

public:
    BaseWindow(const char* windowName, EditorWindowConfig&  windowToggle, Scene* scene): 
            m_windowName(windowName), m_WindowToggle(windowToggle), GScene(scene) {}

    void CloseWindow();

    void Draw();
};