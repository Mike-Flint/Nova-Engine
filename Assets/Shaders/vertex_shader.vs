#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTex;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;



out vec3 crntPos;
out vec3 Normal;
out vec2 TexCoord;
out vec3 Tangent;
out vec3 Bitangent;


uniform mat4 camMatrix;
uniform mat4 model;

void main() {
    crntPos = vec3(model * vec4(aPos, 1.0f));  
    Normal = mat3(transpose(inverse(model))) * aNormal;
    TexCoord = aTex; 
    Tangent = aTangent;  
    Bitangent = aBitangent;

    gl_Position = camMatrix * vec4(crntPos, 1.0);
    
}
