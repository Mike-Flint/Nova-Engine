#pragma once

#include "imgui.h"
#include "EditorWindowState.hpp"
#include "imfilebrowser.h"
#include "Components.hpp"
#include "AppWindow.hpp"

namespace ImWindows{
    class TitleBar{
    private:
        EditorWindows *m_State;

        void maximizeOrRestoreWindow();
        void drawWindowButtons();
        void handleShortcuts();
        void handleWindowDrag();

        glm::ivec2 m_preMaximizePos;
        glm::ivec2 m_preMaximizeSize;
        glm::ivec2 m_dragOffset{0, 0};

        bool m_isDraggingWindow = false;
        
    public:

        TitleBar(EditorWindows &state) : m_State(&state){}
        
        void Draw();
    };
}
