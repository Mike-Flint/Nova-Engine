#version 330 core

layout (triangles) in;                     // Вхідний тип примітива — трикутники
layout (triangle_strip, max_vertices = 3) out; // Вихідний тип примітива — стрічка трикутників

in vec3 fragColor[]; // Приклад передачі даних від вершинного шейдера

void main() {
    // Обробка кожної вершини
    for (int i = 0; i < 3; ++i) {
        gl_Position = gl_in[i].gl_Position; // Передаємо позицію
        geomColor = fragColor[i];          // Передаємо колір
        EmitVertex();                      // Видаємо вершину
    }
    EndPrimitive(); // Завершуємо формування трикутника
}
