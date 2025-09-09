#pragma once

#include <glad/glad.h>
#include "Shader.hpp"
#include "stb_image.h"

class Texture {
public:
    GLuint ID;
    const char* Type;
    GLuint Unit;
    const char* Path;

    Texture(const char* image,
            const char* texType,
            const GLuint& slot,
            const GLenum& format,
            const GLenum& pixelType);

    void TexUnit(Shader& shader,
                const char* uniform,
                const GLuint& unit);
    void Bind();  
    void Unbind();   
    void Delete();
};
