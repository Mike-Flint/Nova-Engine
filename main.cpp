
#include "UIManager.h"
#include "GState.h"


int main() {
    GState::winSize = glm::ivec2(1660, 1240);
    // GState::vpPerc = glm::ivec2(80, 80);
    // GState::vpSize = glm::ivec2(GState::winSize.width * (GState::vpPerc.width / 100.0f), GState::winSize.height * (GState::vpPerc.height/ 100.0f));
    

    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
    
    // Налаштування версії OpenGL (3.3 Core).
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
    // glfwWindowHint(GLFW_RESIZABLE, 0); // Заборона зміни розміру вікна.
    
    GLFWwindow* window = glfwCreateWindow(GState::winSize.width, GState::winSize.height, "OpenGL", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    GState::window = window;
    GState::update();

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glfwSwapInterval(0);

    UIManager ui;

    GLuint error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "OpenGL Error: " << error << std::endl;
    }

    // Увімкнення змішування для прозорості.
    // glEnable(GL_BLEND);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        GState::update();

        glEnable(GL_DEPTH_TEST);
        ui.imGui.newFrame();

        // ui.render.draw();

        ui.drawUI();

        

        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable){
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ui.imGui.destroy();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
