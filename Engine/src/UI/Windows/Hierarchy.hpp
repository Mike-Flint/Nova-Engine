#pragma once

#include "BaseWindow.hpp"

namespace ImWindows{
    class Hierarchy : public BaseWindow{
    private:
        void DrawContent() override;

    public:
        Hierarchy(EditorWindows &state, Scene* GScene) :
            BaseWindow("Hierarchy", state.Hierarchy, GScene){}
    };
}