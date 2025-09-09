#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "imgui.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> 

#include "AppWindow.hpp"
#include "Shader.hpp"
#include "Component.hpp"

// class Component {
// public:
//     Scene* Scene = nullptr;

//     int ID = 0;
    
//     glm::vec3 Position = {0, 0, 0};
//     glm::vec3 Rotation = {0, 0, 0};
//     glm::vec3 Scale = {1, 1, 1};

//     std::vector<std::unique_ptr<Component>> childComponents;
//     std::unordered_map<int, Component*> childLookup;



class Camera : public Component{
public:
    float FOVdeg = 45.0f;
    float NearPlane = 0.1f;
    float FarPlane = 100.0f;

    
    glm::vec3 Up = glm::vec3(0.0f, 0.0f, 1.0f);
    glm::mat4 CameraMatrix = glm::mat4(1.0f);

    Camera() = default;
    Camera(glm::ivec2 size, glm::ivec2 renderPosition);

    void UpdateMatrix();
    void UpdateSizeWindow(glm::ivec2& x);
    void Matrix(Shader& shader, const char* uniform);

    glm::mat4 GetViewMatrix() const {
        return glm::lookAt(Position, Position + Orientation, Up);
    }

    glm::mat4 GetProjectionMatrix() const {
        return glm::perspective(glm::radians(FOVdeg), m_Width / (float)m_Height, NearPlane, FarPlane);
    }



    void Inputs();

private:
    glm::vec3 Orientation = glm::vec3(-1.0f, 0.0f, 0.0f);
    int m_Width;
    int m_Height;

    float m_Speed = 0.1f;
    float m_Sensitivity = 0.1f;

    bool m_FirstClick = true;
    bool m_IsMousePressedInsideWindow = true;

    glm::ivec2 m_RenderPosition;
};