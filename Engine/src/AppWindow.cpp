#include "AppWindow.hpp"

void AppWindow::Init(const char* nameWindow ,glm::ivec2 winSize){
    if(!once) return;

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
        throw std::runtime_error("Failed to create GLFW Window");
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(Window);
    glfwSwapInterval(0);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        throw std::runtime_error("Failed to initialize GLAD");
        return;
    }

    once = false;
}

bool AppWindow::Update(){
    glfwPollEvents();
    glfwSwapBuffers(Window);
    return !glfwWindowShouldClose(Window);
}

// ---------------------Window size---------------------
glm::ivec2 AppWindow::GetWindowSize(){
    glm::ivec2 size;
    glfwGetWindowSize(Window, &size.width, &size.height);
    return size;
}

void AppWindow::SetWindowSize(int width, int height){
    glfwSetWindowSize(Window, width, height);
}

// ------------------Window position------------------
glm::ivec2 AppWindow::GetWindowPos(){
    glm::ivec2 pos;
    glfwGetWindowPos(Window, &pos.x, &pos.y);
    return pos;
}

void AppWindow::SetWindowPos(int xpos, int ypos){
    glfwSetWindowPos(Window, xpos, ypos);
}

// ------------------Cursor position------------------
glm::dvec2 AppWindow::GetCursorPos(){
    glm::dvec2 pos;
    glfwGetCursorPos(Window, &pos.x, &pos.y);
    return pos;
}

void AppWindow::SetCursorPos(double xpos, double ypos){
    glfwSetCursorPos(Window, xpos, ypos);
}

// ------------------Set Window Maximized------------------
void AppWindow::SetFullscreen(){
    GLFWmonitor* monitor = findCurrentMonitor();
    if (!monitor) return;

    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    if (!mode) return;

    glfwSetWindowMonitor(Window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
}

void AppWindow::SetMaximized(){
    GLFWmonitor* monitor = findCurrentMonitor();
    if (monitor) {
        int workX, workY, workWidth, workHeight;

        glfwGetMonitorWorkarea(monitor, &workX, &workY, &workWidth, &workHeight);

        AppWindow::SetWindowPos(workX, workY);
        AppWindow::SetWindowSize(workWidth, workHeight + 1);
    }
}

int AppWindow::GetKey(int key){
    return glfwGetKey(Window, key);
}

void AppWindow::SetCursorMode(int value){
    glfwSetInputMode(Window, GLFW_CURSOR, value);
}

void AppWindow::SetWindowShouldClose(){
    glfwSetWindowShouldClose(Window, GLFW_TRUE);
}

void AppWindow::IconifyWindow(){
    glfwIconifyWindow(Window);
}

GLFWmonitor* AppWindow::findCurrentMonitor() {
    int monitorCount;
    GLFWmonitor** monitors = glfwGetMonitors(&monitorCount);

    if (monitorCount == 0) {
        return nullptr;
    }

    glm::ivec2 pos = GetWindowPos();
    glm::ivec2 size = GetWindowSize();

    GLFWmonitor* bestMonitor = nullptr;
    long long maxOverlap = 0;

    for (int i = 0; i < monitorCount; i++) {
        GLFWmonitor* monitor = monitors[i];
        
        // Get the monitor's workspace (including the taskbar)
        int monX, monY, monWidth, monHeight;
        glfwGetMonitorWorkarea(monitor, &monX, &monY, &monWidth, &monHeight);

        // Calculate the cross-sectional area
        int overlapX = glm::max((int)pos.x, monX);
        int overlapY = glm::max((int)pos.y, monY);

        int overlapRight = glm::min((int)(pos.x + size.x), monX + monWidth);
        int overlapBottom = glm::min((int)(pos.y + size.y), monY + monHeight);

        long long overlapWidth = overlapRight - overlapX;
        long long overlapHeight = overlapBottom - overlapY;

        if (overlapWidth > 0 && overlapHeight > 0) {
            long long overlapArea = overlapWidth * overlapHeight;
            
            // If this monitor has a larger cross-sectional area, it becomes the best candidate
            if (overlapArea > maxOverlap) {
                maxOverlap = overlapArea;
                bestMonitor = monitor;
            }
        }
    }
    
    // If the window is completely outside all monitors, return the primary monitor
    if (bestMonitor == nullptr) {
        return glfwGetPrimaryMonitor();
    }

    return bestMonitor;
}


void AppWindow::Destroy(){
    glfwDestroyWindow(Window);
    glfwTerminate();
}