#include "Texture.hpp" // Підключення заголовочного файлу класу Texture.

// Конструктор класу Texture для завантаження текстури.
Texture::Texture(const char* image, const char* texType, const GLuint& slot, const GLenum& format, const GLenum& pixelType) {
    type = texType; // Зберігаємо тип текстури.
    path = image;

    // Змінні для зберігання параметрів зображення.
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // Перевернути зображення по вертикалі.

    // Завантаження зображення.
    unsigned char* dataImg = stbi_load(image, &width, &height, &nrChannels, 0);
    if (!dataImg) { // Перевірка на помилки завантаження.
        std::cerr << "Failed to load image: " << stbi_failure_reason() << std::endl;
        return;
    }

    glGenTextures(1, &ID); // Генерація текстури та отримання її ID.
    glActiveTexture(GL_TEXTURE0 + slot); // Активація слоту текстури.
    unit = slot;
    glBindTexture(GL_TEXTURE_2D, ID); // Прив'язка текстури до її типу.

    // Налаштування параметрів фільтрації текстури.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR); // Фільтрація для зменшення текстури.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // Фільтрація для збільшення текстури.

    // Налаштування параметрів обгортання текстури.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Повторення текстури по осі S.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Повторення текстури по осі T.

    // Установка кольору для рамки (border color).
    // std::vector<float> colorBorder{ 0.5f, 0.3f, 0.7f, 0.9f }; // RGBA кольори.
    // glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, colorBorder.data());

    // Визначення формату зображення залежно від кількості каналів.
    GLenum imageFormat = (nrChannels == 4) ? GL_RGBA : (nrChannels == 3) ? GL_RGB : GL_RED;
    std::cout << width << " | " << height << std::endl; // Вивід розмірів зображення.

    // Завантаження даних зображення в текстуру.
    glTexImage2D(GL_TEXTURE_2D, 0, imageFormat, width, height, 0, imageFormat, pixelType, dataImg);

    glGenerateMipmap(GL_TEXTURE_2D); // Генерація міпмапів для текстури.
    stbi_image_free(dataImg); // Звільнення пам'яті, зайнятої зображенням.
    glBindTexture(GL_TEXTURE_2D, 0); // Відв'язка текстури.
}

// Прив'язка текстури до уніформної змінної шейдера.
void Texture::texUnit(Shader& shader, const char* uniform, const GLuint& unit) {
    GLuint texUni = glGetUniformLocation(shader.ID, uniform); // Отримання локації уніформної змінної.
    shader.Activate(); // Активація шейдера.
    glUniform1i(texUni, unit); // Прив'язка текстурного юніту.
}

// Прив'язка текстури.
void Texture::Bind() {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, ID); // Прив'язка текстури за її типом і ID.
}

// Відв'язка текстури.
void Texture::Unbind() {
    glBindTexture(GL_TEXTURE_2D, 0); // Відв'язка текстури (встановлення 0).
}

// Видалення текстури.
void Texture::Delete() {
    glDeleteTextures(1, &ID); // Видалення текстури за її ID.
}
