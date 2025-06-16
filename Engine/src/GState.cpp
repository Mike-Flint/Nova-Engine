#include "GState.h"

GLFWwindow* GState::window = nullptr;

glm::dvec2 GState::mouse = glm::dvec2();
glm::ivec2 GState::winPos = glm::ivec2();
glm::ivec2 GState::winSize = glm::ivec2();
glm::ivec2 GState::vpPerc = glm::ivec2();
glm::ivec2 GState::vpSize = glm::ivec2();



bool GState::isMouseBusy = false;
bool GState::isWindowMaximized = false;

void GState::update(){
    glfwGetCursorPos(window,  &mouse.x,        &mouse.y);
    glfwGetWindowSize(window, &winSize.width,  &winSize.height);
    glfwGetWindowPos(window,  &winPos.x,       &winPos.y);
    

    GState::vpSize = glm::ivec2(GState::winSize.width * (vpPerc.width / 100.0f), GState::winSize.height * (vpPerc.height/ 100.0f));

    // if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE){
    //     isMouseBusy = false;
    // }
}