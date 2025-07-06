#include "EngineCore.hpp"

EngineCore::EngineCore(EngineWindowGLFW& win){
    window = &win;
    GState::window = win.Window;
    GState::update();
}

void EngineCore::Run(){
    while (window->Update()){
        GState::update();
        glEnable(GL_DEPTH_TEST);

        ui.imGui.newFrame();

        ui.drawUI();
    }
}

EngineCore::~EngineCore(){
}