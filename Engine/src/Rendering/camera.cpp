#include "Camera.h"

Camera::Camera(glm::ivec2& size, glm::ivec2 renderPosition, glm::vec3 position)
    : m_width(size.width), m_height(size.height), m_renderPosition(renderPosition), position(position) {}

void Camera::init(glm::ivec2& size, glm::ivec2 renderPosition, glm::vec3 position){
    m_width = size.width;
    m_height = size.height;
    m_renderPosition = renderPosition;
    position = position;
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

    if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS){
        double mouseX;
        double mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);

        bool insideViewport = (mouseX >= m_renderPosition.x + 10) && (mouseX <= m_renderPosition.x + m_width) &&
                            (mouseY >= m_renderPosition.y) && (mouseY <= m_renderPosition.y + m_height);
        if(insideViewport && m_isMousePressedInsideWindow){
            if (m_firstClick){
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
                glfwSetCursorPos(window, (m_width / 2), (m_height / 2));
                m_firstClick = false;
            }

            glfwGetCursorPos(window, &mouseX, &mouseY);

            float rotX = m_sensitivity * (float)(mouseY - (m_height / 2)) / m_height;
            float rotY = m_sensitivity * (float)(mouseX - (m_width / 2)) / m_width;

            glm::vec3 newOrientation = glm::rotate(orientation, glm::radians(-rotX), glm::normalize(glm::cross(orientation, up)));

            if (abs(glm::angle(newOrientation, up) - glm::radians(90.0f)) <= glm::radians(85.0f)){
                orientation = newOrientation;
            }
            //left and right
            orientation = glm::rotate(orientation, glm::radians(-rotY), up);

            glfwSetCursorPos(window, (m_width / 2), (m_height / 2));
        }else{
            m_isMousePressedInsideWindow = false;
        }

    }

    if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE){

        m_isMousePressedInsideWindow = true;
        if (!m_firstClick){
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            m_firstClick = true;
        }
    }
}

