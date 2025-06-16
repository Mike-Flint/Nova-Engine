#include "ImGuiRender.h"


// int GetTaskbarHeight() {
//     APPBARDATA abd;
//     abd.cbSize = sizeof(APPBARDATA);
//     HWND taskbar = FindWindow("Shell_TrayWnd", NULL);
//     abd.hWnd = taskbar;
//     if (SHAppBarMessage(ABM_GETTASKBARPOS, &abd)) {
//         RECT rc = abd.rc;
//         return abs(rc.bottom - rc.top); // висота або ширина панелі
//     }
//     return 0;
// }

void ImGuiRender::init(){
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(GState::window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

void ImGuiRender::newFrame(){
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void ImGuiRender::render() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiRender::destroy(){
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
