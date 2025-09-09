#include "Hierarchy.hpp"

void ImWindows::Hierarchy::DrawContent(){
    static int count = 0;
    static float framerate = 0;

    count++;
    if (count > 100){
        framerate = ImGui::GetIO().Framerate;
        count = 0;
    }
    ImGui::Text("FPS: %.1f", framerate);
}
