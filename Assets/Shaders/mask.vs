
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal; // Вхід нормалі

out vec3 FragNormal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 camMatrix;

void main() {
    FragPos = vec3(model * vec4(aPos, 1.0)); 
    FragNormal = mat3(transpose(inverse(model))) * aNormal;
    gl_Position = camMatrix * vec4(FragPos, 1.0);
}
