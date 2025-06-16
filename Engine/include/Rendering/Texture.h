#pragma once

#include <glad/glad.h>
#include "Shader.h"
#include "stb_image.h" // Завантаження зображень як текстур.

class Texture {
public:
    GLuint ID;
    const char* type;
    GLuint unit;
    const char* path;

    // Texture();

    Texture(const char* image, const char* texType, const GLuint& slot, const GLenum& format, const GLenum& pixelType);

    void texUnit(Shader& shader, const char* uniform, const GLuint& unit);
    void Bind();  
    void Unbind();   
    void Delete();
};
