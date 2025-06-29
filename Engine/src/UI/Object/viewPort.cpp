#include "ViewPort.hpp"

void WindowViewPort::draw(){
    ImGui::Begin("Viewport");

    ImVec2 viewportSize = ImGui::GetContentRegionAvail();
    
    if (viewportSize.x > 0 && viewportSize.y > 0) {
        render.draw(glm::ivec2(viewportSize.x, viewportSize.y));

        ImGui::Image(
            (void*)(intptr_t)render.viewportFBO.textureId, 
            viewportSize,
            ImVec2(0, 1), 
            ImVec2(1, 0)
        );
    }



    ImGui::End();
}