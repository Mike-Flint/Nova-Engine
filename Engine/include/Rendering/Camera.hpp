#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "imgui.h"
#include "GState.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> 

#include "Shader.hpp"



class Camera{
public:
    glm::vec3 position;
    glm::vec3 orientation = glm::vec3(-1.0f, 0.0f, 0.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::mat4 cameraMatrix = glm::mat4(1.0f);

    Camera() = default;
    Camera(glm::ivec2& size, glm::ivec2 renderPosition, glm::vec3 position);
    void init(glm::ivec2& size, glm::ivec2 renderPosition, glm::vec3 position);

    void updateMatrix(float FOVdeg, float nearPlane, float farPlane);
    void updateSizeWindow(glm::ivec2& x);
    void matrix(Shader& shader, const char* uniform);

    void inputs(GLFWwindow* window);

private:
    int m_width;
    int m_height;

    float m_speed = 0.1f;
    float m_sensitivity = 0.1f;

    bool m_firstClick = true;
    bool m_isMousePressedInsideWindow = true;

    glm::ivec2 m_renderPosition;
};