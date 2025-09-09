#pragma once

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <vector>

#define MAX_BONE_INFLUENCE 4

struct Vertex {
    glm::vec3 Position;       
    glm::vec3 Normal;         
    glm::vec3 Tangent;        
    glm::vec2 TexCoords;      
    glm::vec3 Bitangent;      
    int m_BoneIDs[MAX_BONE_INFLUENCE];
    float m_Weights[MAX_BONE_INFLUENCE];
};

class VBO {
public:
    GLuint ID;

    VBO(std::vector<Vertex>& vertices);

    void Bind();  
    void Unbind();   
    void Delete();
};


