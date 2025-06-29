#include "EBO.hpp" // Підключення заголовочного файлу класу EBO.

// Конструктор класу EBO (Element Buffer Object).
EBO::EBO(std::vector<GLuint>& indices) {
    glGenBuffers(1, &ID); // Генерація буфера та отримання його ID.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID); // Прив'язка буфера як буфера елементів.
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW); // Заповнення буфера даними.
}

// Метод для прив'язки EBO.
void EBO::Bind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID); // Прив'язка буфера елементів.
}

// Метод для відв'язки EBO.
void EBO::Unbind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // Відв'язка буфера елементів (встановлення 0).
}

// Метод для видалення EBO.
void EBO::Delete() {
    glDeleteBuffers(1, &ID); // Видалення буфера елементів за його ID.
}
