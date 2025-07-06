#include "GState.hpp"

GLFWwindow* GState::window = nullptr;

glm::dvec2 GState::mouse = glm::dvec2();
glm::ivec2 GState::winPos = glm::ivec2();
glm::ivec2 GState::winSize = glm::ivec2();

bool GState::isMouseBusy = false;
bool GState::isWindowMaximized = false;

void GState::update(){
    glfwGetCursorPos(window,  &mouse.x,        &mouse.y);
    glfwGetWindowSize(window, &winSize.width,  &winSize.height);
    glfwGetWindowPos(window,  &winPos.x,       &winPos.y);
}