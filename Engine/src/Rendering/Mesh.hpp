#pragma once

#include <string>

#include "VAO.hpp"
#include "EBO.hpp"
#include "FBO.hpp"
#include "Components/Camera.hpp"
#include "stb_image.h" 


struct Texture {
    unsigned int ID;
    std::string Type;
    std::string Path;
};

class Mesh{
public:
    std::vector<Vertex> Vertices;
    std::vector<GLuint> Indices;
    std::vector<Texture> Textures;

    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 rotation = glm::vec3(0.0f);
    glm::vec3 scale = glm::vec3(1.0f);
    
    glm::mat4 model = glm::mat4(1.0f);

    VAO VAO;

    Mesh( std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::vector<Texture>& textures);
                                                        
    void Draw(Shader& shader, Camera& camera);
};