#pragma once

#include "UIManager.hpp"
#include "EngineWindowGLFW.hpp"
#include "GState.hpp"

class EngineCore{

public:
    EngineCore(EngineWindowGLFW& win);

    void Run();

    ~EngineCore();
    
private:
    EngineWindowGLFW* window;
    UIManager ui;

};