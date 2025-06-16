#pragma once

#include <string>

#include "VAO.h"
#include "EBO.h"
#include "FBO.h"
#include "Camera.h"
#include "stb_image.h" 
// #include "Texture.h"

struct Texture {
    unsigned int ID;
    std::string type;
    std::string path;  // we store the path of the texture to compare with other textures
};


// unsigned int TextureFromFile(const char *path, const std::string &directory, bool gamma);


class Mesh{
public:
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;

    VAO VAO;

    Mesh( std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::vector<Texture>& textures);
                                                        
    void Draw(Shader& shader, Camera& camera);
};