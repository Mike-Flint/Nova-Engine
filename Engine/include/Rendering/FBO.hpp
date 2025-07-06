#pragma once

#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>

class FBO {
public:
    GLuint fboID;
    GLuint textureId;
    GLuint depthId;

    FBO() = default;
    FBO(glm::ivec2 size, GLenum format = GL_RGB, bool hasDepth = false);
    void init(glm::ivec2 size, GLenum format = GL_RGB, bool hasDepth = false);
    ~FBO();
    void updateSizeWindow(glm::ivec2 size);

    void Bind();
    void Unbind();

private:
    bool isInitialized = false;
    void Initialize();

    GLenum m_formatTexture;

    int m_width, m_height;
    bool m_hasDepth;
};

