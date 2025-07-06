#include <EngineWindowGLFW.hpp>

EngineWindowGLFW::EngineWindowGLFW(const char* nameWindow ,glm::ivec2 winSize){
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW");
        return;
    }   
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

    Window = glfwCreateWindow(winSize.width, winSize.height, nameWindow, nullptr, nullptr);
    if (!Window) {
        throw std::runtime_error("Failed to create GLFW window");
        glfwTerminate();
        return;
    }

    GState::window = Window;
    GState::update();

    glfwMakeContextCurrent(Window);
    glfwSwapInterval(0);
}

bool EngineWindowGLFW::Update(){

    glfwPollEvents();
    glfwSwapBuffers(Window);
    return !glfwWindowShouldClose(Window);
}

EngineWindowGLFW::~EngineWindowGLFW(){
    glfwDestroyWindow(Window);
    glfwTerminate();
}