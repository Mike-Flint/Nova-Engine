#pragma once

#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>

class FBO {
public:
    GLuint FboID;
    GLuint TextureID;
    GLuint DepthID;

    FBO() = default;

    FBO(glm::ivec2 size, GLenum format = GL_RGB, bool hasDepth = false);

    void Bind();
    void Unbind();
    void UpdateSizeWindow(glm::ivec2 size);
    ~FBO();

private:
    void Initialize();

    GLenum m_FormatTexture;

    int m_Width, m_Height;
    bool m_HasDepth;
};

