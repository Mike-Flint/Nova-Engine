#pragma once

#include <glad/glad.h>
#include "VBO.hpp"

class VAO {
public:
    GLuint ID;

    VAO();

    void LinkAttrib(VBO& VBO,
                    const GLuint& layout,
                    const GLuint& numComponents,
                    const GLenum& type,
                    const GLsizei& stride,
                    void* offset);
    void Bind();  
    void Unbind();   
    void Delete();
};

