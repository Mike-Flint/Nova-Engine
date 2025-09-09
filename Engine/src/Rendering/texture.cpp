#include "Texture.hpp"

Texture::Texture(const char* image, const char* texType, const GLuint& slot, const GLenum& format, const GLenum& pixelType) {
    Type = texType;
    Path = image;

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);

    unsigned char* dataImg = stbi_load(image, &width, &height, &nrChannels, 0);
    if (!dataImg) {
        std::cerr << "Failed to load image: " << stbi_failure_reason() << std::endl;
        return;
    }

    glGenTextures(1, &ID);
    glActiveTexture(GL_TEXTURE0 + slot);
    Unit = slot;
    glBindTexture(GL_TEXTURE_2D, ID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    GLenum imageFormat = (nrChannels == 4) ? GL_RGBA : (nrChannels == 3) ? GL_RGB : GL_RED;
    std::cout << width << " | " << height << std::endl;

    glTexImage2D(GL_TEXTURE_2D, 0, imageFormat, width, height, 0, imageFormat, pixelType, dataImg);

    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(dataImg);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::TexUnit(Shader& shader, const char* uniform, const GLuint& unit) {
    GLuint texUni = glGetUniformLocation(shader.ID, uniform);
    shader.Activate();
    glUniform1i(texUni, unit);
}

void Texture::Bind() {
    glActiveTexture(GL_TEXTURE0 + Unit);
    glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::Unbind() {
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Delete() {
    glDeleteTextures(1, &ID);
}
