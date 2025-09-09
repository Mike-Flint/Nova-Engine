
#include "UIManager.hpp"
#include "AppWindow.hpp"

int main() {

    AppWindow::Init("Nova Engine", glm::ivec2(1280, 720));

    UIManager ui;

    while (AppWindow::Update()){
        ui.drawUI();
    }

    AppWindow::Destroy();
    return 0;
}
