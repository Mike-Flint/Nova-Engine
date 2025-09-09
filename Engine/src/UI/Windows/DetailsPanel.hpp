#pragma once

#include "BaseWindow.hpp"
#include "Renderer.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/euler_angles.hpp>

namespace ImWindows{
    class DetailsPanel : public BaseWindow{
    private:
        void DrawContent() override;
    public:
        DetailsPanel(EditorWindows &state, Scene* GScene) :
            BaseWindow("Details", state.DetailsPanel, GScene){}
    };
}