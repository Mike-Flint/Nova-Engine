#version 330 core
layout (location = 0) in vec2 aPos;      // Отримуємо позицію з VAO (xy)
layout (location = 2) in vec2 aTexCoord; // Отримуємо текстурні координати з VAO (uv)

out vec2 TexCoords;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
    TexCoords = aTexCoord; // Просто передаємо отримані текстурні координати
}