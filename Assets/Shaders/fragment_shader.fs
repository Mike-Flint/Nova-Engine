#version 330 core
out vec4 FragColor;


in vec3 crntPos;
in vec3 Normal;
in vec2 TexCoord;
in vec3 Tangent;
in vec3 Bitangent;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_height1;


uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;

vec4 pointLight(){
    vec3 lightVec = lightPos - crntPos;
    float dist = length(lightVec);

    float a = 1.0f;
    float b = 0.4f;

    float inten = 1 / (a * dist * dist + b * dist + 1.0f);

    float amdient = 0.50f;

    vec3 normal = normalize(Normal);
    vec3 lightDirection = normalize(lightVec);

    float diffuse = max(dot(normal, lightDirection), 0.0f);

    float specularLight = 0.50f;
    vec3 viewDirection = normalize(camPos - crntPos);
    vec3 reflectDirection = reflect(-lightDirection, normal);
    float specAmount = pow(max(dot(viewDirection, reflectDirection), 0.0f), 16);
    float specular = specAmount * specularLight;

    return (texture(texture_diffuse1, TexCoord) * (diffuse * inten + amdient) + texture(texture_specular1, TexCoord).r * specular * inten) * lightColor;

}

vec4 directLight(){
    float amdient = 0.20f;
    vec3 lightVec = lightPos - crntPos;

    vec3 normal = normalize(Normal);
    vec3 lightDirection = normalize(vec3(1.5f, 1.0f, 1.0f));

    float diffuse = max(dot(normal, lightDirection), 0.0f);

    float specularLight = 0.50f;
    vec3 viewDirection = normalize(camPos - crntPos);
    vec3 reflectDirection = reflect(-lightDirection, normal);
    float specAmount = pow(max(dot(viewDirection, reflectDirection), 0.0f), 16);
    float specular = specAmount * specularLight;

    return (texture(texture_diffuse1, TexCoord) * (diffuse + amdient) + texture(texture_specular1, TexCoord).r * specular) * lightColor;
}

vec4 spotLight(){

    float otherCone = 0.90f;
    float innerCone = 0.95f;

    float amdient = 0.20f;

    vec3 normal = normalize(Normal);
    vec3 lightDirection = normalize(lightPos - crntPos);

    float diffuse = max(dot(normal, lightDirection), 0.0f);

    float specularLight = 0.50f;
    vec3 viewDirection = normalize(camPos - crntPos);
    vec3 reflectDirection = reflect(-lightDirection, normal);
    float specAmount = pow(max(dot(viewDirection, reflectDirection), 0.0f), 16);
    float specular = specAmount * specularLight;

    float angle = dot(vec3(-0.0f, -1.0f, -0.0f), -lightDirection);
    float inten = clamp((angle - otherCone) / (innerCone - otherCone), 0.0f, 1.0f);

    return (texture(texture_diffuse1, TexCoord) * (diffuse * inten + amdient) + texture(texture_specular1, TexCoord).r * specular * inten) * lightColor;
}

void main() {
    FragColor = directLight();
}
