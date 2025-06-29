#include "Camera.hpp"

Camera::Camera(glm::ivec2& size, glm::ivec2 renderPosition, glm::vec3 position)
    : m_width(size.width), m_height(size.height), m_renderPosition(renderPosition), position(position) {}

void Camera::init(glm::ivec2& size, glm::ivec2 renderPosition, glm::vec3 position){
    m_width = size.width;
    m_height = size.height;
    m_renderPosition = renderPosition;
    this->position = position;
}

void Camera::updateMatrix(float FOVdeg, float nearPlane, float farPlane) {
    glm::mat4 view = glm::lookAt(position, position + orientation, up);
    glm::mat4 projection = glm::perspective(glm::radians(FOVdeg), (float)m_width / m_height, nearPlane, farPlane);
    cameraMatrix = projection * view;
}

void Camera::matrix(Shader& shader, const char* uniform){
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(cameraMatrix));
}

void Camera::updateSizeWindow(glm::ivec2& size){
    m_width = size.width;
    m_height = size.height;
}

void Camera::inputs(GLFWwindow* window){
    if(ImGui::IsWindowFocused()){
        if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
            position += m_speed * orientation;
        }
        if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
            position += m_speed * -glm::normalize(glm::cross(orientation, up));
        }
        if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
            position += m_speed * -orientation;
        }
        if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
            position += m_speed * glm::normalize(glm::cross(orientation, up));
        }
        if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){
            position += m_speed * up;
        }
        if(glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS){
            position += m_speed * -up;
        }
        if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS){
            m_speed = 0.4f;
        }
        if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE){
            m_speed = 0.1f;
        }
    }

    if(ImGui::IsMouseDown(ImGuiMouseButton_Left)){
        ImVec2 size = ImGui::GetWindowSize();
        
        ImVec2 position = ImGui::GetWindowPos();
    
        glm::dvec2 mouse;
        glfwGetCursorPos(window, &mouse.x, &mouse.y);

        bool insideViewport = (mouse.x >= position.x - GState::winPos.x) && (mouse.x <= position.x - GState::winPos.x + size.x) &&
                            (mouse.y >= position.y - GState::winPos.y) && (mouse.y <= position.y - GState::winPos.y + size.y);

        if(insideViewport && m_isMousePressedInsideWindow && ImGui::IsWindowHovered()){
            if (m_firstClick){
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
                glfwSetCursorPos(window, position.x - GState::winPos.x + (size.x / 2), position.y - GState::winPos.y + (size.y / 2));
                glfwGetCursorPos(window, &mouse.x, &mouse.y);
                m_firstClick = false;
            }

            int saveY = (float)((position.y - GState::winPos.y + (size.y / 2)));
            int saveX = (float)((position.x - GState::winPos.x + (size.x / 2)));

            float rotX = m_sensitivity * (mouse.y - saveY);
            float rotY = m_sensitivity * (mouse.x - saveX);

            glm::vec3 newOrientation = glm::rotate(orientation, glm::radians(-rotX), glm::normalize(glm::cross(orientation, up)));

            if (abs(glm::angle(newOrientation, up) - glm::radians(90.0f)) <= glm::radians(85.0f)){
                orientation = newOrientation;
            }
            //left and right
            orientation = glm::rotate(orientation, glm::radians(-rotY), up);
            glfwSetCursorPos(window,  position.x - GState::winPos.x + (size.x / 2) , position.y - GState::winPos.y + (size.y / 2));
        }else{
            m_isMousePressedInsideWindow = false;
        }
    }else{
        m_isMousePressedInsideWindow = true;
        if (!m_firstClick){
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            m_firstClick = true;
        }
    }
    

}

