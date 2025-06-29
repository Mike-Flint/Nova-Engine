#pragma once

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <vector>

#define MAX_BONE_INFLUENCE 4

struct Vertex {
    glm::vec3 Position;       // Позиція вершини у просторі (x, y, z)
    glm::vec3 Normal;         // Нормаль вершини (для освітлення)
    glm::vec3 Tangent;        // Тангенти (для Normal Mapping)
    glm::vec2 TexCoords;      // Текстурні координати (UV)
    glm::vec3 Bitangent;      // Бітангенти (перпендикулярні до Normal і Tangent)
    int m_BoneIDs[MAX_BONE_INFLUENCE]; // Ідентифікатори кісток (для скелетної анімації)
    float m_Weights[MAX_BONE_INFLUENCE]; // Ваги впливу кожної кістки
};



class VBO {
public:
    GLuint ID;

    VBO(std::vector<Vertex>& vertices);

    void Bind();  
    void Unbind();   
    void Delete();
};


