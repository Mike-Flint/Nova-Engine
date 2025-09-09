#include "Camera.hpp"

Camera::Camera(glm::ivec2 size, glm::ivec2 renderPosition)
    : m_Width(size.width), m_Height(size.height), m_RenderPosition(renderPosition){}


void Camera::UpdateMatrix() {
    glm::mat4 view = glm::lookAt(Position, Position + Orientation, Up);
    glm::mat4 projection = glm::perspective(glm::radians(FOVdeg), (float)m_Width / m_Height, NearPlane, FarPlane);
    CameraMatrix = projection * view;
}

void Camera::Matrix(Shader& shader, const char* uniform){
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(CameraMatrix));
}

void Camera::UpdateSizeWindow(glm::ivec2& size){
    m_Width = size.width;
    m_Height = size.height;
}

void Camera::Inputs(){
    if(ImGui::IsMouseDown(ImGuiMouseButton_Right) && ImGui::IsWindowHovered()){
        if(AppWindow::GetKey(GLFW_KEY_W) == KEY_PRESS){
            Position += m_Speed * Orientation;
        }
        if(AppWindow::GetKey(GLFW_KEY_A) == KEY_PRESS){
            Position += m_Speed * -glm::normalize(glm::cross(Orientation, Up));
        }
        if(AppWindow::GetKey(GLFW_KEY_S) == KEY_PRESS){
            Position += m_Speed * -Orientation;
        }
        if(AppWindow::GetKey(GLFW_KEY_D) == KEY_PRESS){
            Position += m_Speed * glm::normalize(glm::cross(Orientation, Up));
        }
        if(AppWindow::GetKey(GLFW_KEY_SPACE) == KEY_PRESS){
            Position += m_Speed * Up;
        }
        if(AppWindow::GetKey(GLFW_KEY_LEFT_CONTROL) == KEY_PRESS){
            Position += m_Speed * -Up;
        }
        if(AppWindow::GetKey(GLFW_KEY_LEFT_SHIFT) == KEY_PRESS){
            m_Speed = 0.4f;
        }
        if(AppWindow::GetKey(GLFW_KEY_LEFT_SHIFT) == KEY_RELEASE){
            m_Speed = 0.1f;
        }
        
        ImVec2 winSize = ImGui::GetWindowSize();
        ImVec2 winPos = ImGui::GetWindowPos();
        ImVec2 mousePos = ImGui::GetMousePos();
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        
        glm::dvec2 mouse;
        mouse = AppWindow::GetCursorPos();

        bool insideViewport = (mousePos.x >= winPos.x) && (mousePos.x <= winPos.x + winSize.x) &&
                            (mousePos.y >= winPos.y) && (mousePos.y <= winPos.y + winSize.y);

        if(insideViewport && m_IsMousePressedInsideWindow ){
            if (m_FirstClick){
                ImGuiIO& io = ImGui::GetIO();
                io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
                AppWindow::SetCursorMode(CURSOR_HIDDEN);

                AppWindow::SetCursorPos(winPos.x + (winSize.x / 2) - viewport->Pos.x,
                                        winPos.y + (winSize.y / 2) - viewport->Pos.y);

                mouse = AppWindow::GetCursorPos();

                m_FirstClick = false;
            }

            int saveY = (int)(winPos.y + (winSize.y / 2) - viewport->Pos.y);
            int saveX = (int)(winPos.x + (winSize.x / 2) - viewport->Pos.x);

            float rotX = (float)(m_Sensitivity * (int)(mouse.y - saveY));
            float rotY = (float)(m_Sensitivity * (int)(mouse.x - saveX));
            

            glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotX), glm::normalize(glm::cross(Orientation, Up)));

            if (abs(glm::angle(newOrientation, Up) - glm::radians(90.0f)) <= glm::radians(85.0f)){
                Orientation = newOrientation;
            }
            //left and right
            Orientation = glm::rotate(Orientation, glm::radians(-rotY), Up);
            AppWindow::SetCursorPos(winPos.x + (winSize.x / 2) - viewport->Pos.x,
                            winPos.y + (winSize.y / 2) - viewport->Pos.y);
        }else{
            m_IsMousePressedInsideWindow = false;
        }
    }else{
        m_IsMousePressedInsideWindow = true;
        if (!m_FirstClick){
            AppWindow::SetCursorMode(CURSOR_NORMAL);
            ImGuiIO& io = ImGui::GetIO();
            io.ConfigFlags &= ~ImGuiConfigFlags_NoMouseCursorChange;
            
            m_FirstClick = true;
        }
    }
    

}

