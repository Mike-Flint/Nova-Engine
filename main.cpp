#include "EngineCore.hpp"


int main() {
    try {
        EngineWindowGLFW window("Nova Engine", glm::ivec2(1280, 720));

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cerr << "Failed to initialize GLAD" << std::endl;
            return 0;
        }

        EngineCore engine(window);

        engine.Run();
        
    } catch (const std::exception& e) {
        std::cerr << "Engine error: " << e.what() << std::endl;
        return -2;
    }

    return 0;
}
