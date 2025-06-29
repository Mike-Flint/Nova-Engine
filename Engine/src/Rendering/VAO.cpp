#include "VAO.hpp" // Підключення заголовочного файлу для класу VAO.

// Конструктор класу VAO, генерує об'єкт вершинного масиву.
VAO::VAO() {
    glGenVertexArrays(1, &ID); // Генерація ID для вершинного масиву (VAO).
}

// Зв'язування об'єкта буфера вершин (VBO) з VAO і налаштування атрибутів вершини.
void VAO::LinkAttrib(VBO& VBO, const GLuint& layout, const GLuint& numComponents, const GLenum& type, const GLsizei& stride, void* offset) {
    VBO.Bind(); // Прив'язка VBO.
    
    // Визначення формату атрибутів вершини.
    glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
    glEnableVertexAttribArray(layout); // Увімкнення атрибуту вершини.

    VBO.Unbind(); // Відв'язка VBO після налаштування.
}

// Прив'язка VAO.
void VAO::Bind() {
    glBindVertexArray(ID); // Прив'язка VAO за його ID.
}

// Відв'язка VAO.
void VAO::Unbind() {
    glBindVertexArray(0); // Відв'язка VAO (встановлення 0).
}

// Видалення VAO.
void VAO::Delete() {
    glDeleteVertexArrays(1, &ID); // Видалення VAO з пам'яті OpenGL.
}
