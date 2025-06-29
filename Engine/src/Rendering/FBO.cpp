#include "FBO.hpp"
#include <iostream>

FBO::FBO(glm::ivec2& size, GLenum format, bool hasDepth)
    : m_width(size.width), m_height(size.height), m_hasDepth(hasDepth),m_formatTexture(format) {
    Initialize();
}

void FBO::init(glm::ivec2& size, GLenum format, bool hasDepth){
    m_width = size.width;
    m_height = size.height;
    m_hasDepth = hasDepth;
    m_formatTexture = format;
    
    Initialize();
}

FBO::~FBO() {
    glDeleteFramebuffers(1, &fboID);
    glDeleteTextures(1, &textureId);
    
    if (m_hasDepth) {
        glDeleteRenderbuffers(1, &depthId);
    }
}
;

void FBO::updateSizeWindow(glm::ivec2& size){
    m_width = size.width;
    m_height = size.height;

    // Перебиндимо фреймбуфер
    glBindFramebuffer(GL_FRAMEBUFFER, fboID);

    // Оновлюємо текстуру
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

    // Якщо є буфер глибини - оновлюємо і його
    if (m_hasDepth) {
        glBindRenderbuffer(GL_RENDERBUFFER, depthId);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_width, m_height);
    }

    // Перевірка статусу фреймбуфера
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "Framebuffer not complete after resize!" << std::endl;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FBO::Initialize() {
    glGenFramebuffers(1, &fboID);
    glBindFramebuffer(GL_FRAMEBUFFER, fboID);

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureId, 0);
    

    // Генерація буфера глибини, якщо потрібно
    if (m_hasDepth) {
        glGenRenderbuffers(1, &depthId);
        glBindRenderbuffer(GL_RENDERBUFFER, depthId);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_width, m_height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthId);
    }

    // Перевірка статусу FBO
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "Framebuffer not complete!" << std::endl;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FBO::Bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, fboID);
    glViewport(0, 0, m_width, m_height);
}

void FBO::Unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
