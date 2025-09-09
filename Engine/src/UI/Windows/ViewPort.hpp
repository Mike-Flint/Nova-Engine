#pragma once

#include "Renderer.hpp"
#include "Scene.hpp"
#include "imgui.h"
#include "imgui_internal.h"
#include "ImGuizmo.h"
#include <glm/gtx/matrix_decompose.hpp>
#include "EditorWindowState.hpp"

namespace ImWindows{
    class ViewPort{
    private:
        
        bool m_once = true;
        EditorWindows *m_state;

        void selectMeshUnderCursor();

    public:
        Scene* GScene;
        Renderer Render;
        ViewPort(EditorWindows &state, Scene* scene) : m_state(&state), GScene(scene), Render(scene){}
        void Draw();
    };
}
