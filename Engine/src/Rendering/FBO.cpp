#include "FBO.hpp"
#include <iostream>

FBO::FBO(glm::ivec2 size, GLenum format, bool hasDepth)
    : m_Width(size.width), m_Height(size.height), m_HasDepth(hasDepth),m_FormatTexture(format) {
    Initialize();
}

FBO::~FBO() {
    glDeleteFramebuffers(1, &FboID);
    glDeleteTextures(1, &TextureID);
    
    if (m_HasDepth) {
        glDeleteRenderbuffers(1, &DepthID);
    }
}

void FBO::UpdateSizeWindow(glm::ivec2 size){
    m_Width = size.width;
    m_Height = size.height;

    glBindFramebuffer(GL_FRAMEBUFFER, FboID);

    glBindTexture(GL_TEXTURE_2D, TextureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

    if (m_HasDepth) {
        glBindRenderbuffer(GL_RENDERBUFFER, DepthID);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_Width, m_Height);
    }

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "Framebuffer not complete after resize!" << std::endl;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FBO::Initialize() {
    glGenFramebuffers(1, &FboID);
    glBindFramebuffer(GL_FRAMEBUFFER, FboID);

    glGenTextures(1, &TextureID);
    glBindTexture(GL_TEXTURE_2D, TextureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, TextureID, 0);

    if (m_HasDepth) {
        glGenRenderbuffers(1, &DepthID);
        glBindRenderbuffer(GL_RENDERBUFFER, DepthID);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_Width, m_Height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, DepthID);
    }

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "Framebuffer not complete!" << std::endl;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FBO::Bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, FboID);
    glViewport(0, 0, m_Width, m_Height);
}

void FBO::Unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
