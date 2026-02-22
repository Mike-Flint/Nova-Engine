#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform sampler2D maskTexture;

void main()
{
    vec2 texSize = textureSize(maskTexture, 0);
    vec2 texelSize = 1.0 / texSize; 

    float center = texture(maskTexture, TexCoords).r;

    int radius = 4;
    float edgeIntensity = 0.0;


    for (int y = -radius; y <= radius; y++) {
        for (int x = -radius; x <= radius; x++) {
            if (x == 0 && y == 0) continue;

            vec2 offset = vec2(x, y) * texelSize;
            vec2 neighborCoords = TexCoords + offset;

            if (neighborCoords.x < 0.0 || neighborCoords.x > 1.0 || 
                neighborCoords.y < 0.0 || neighborCoords.y > 1.0) 
            {
                continue;
            }

            float neighbor = texture(maskTexture, neighborCoords).r;

            if (neighbor != center) {
                float distance = length(vec2(x, y));
                edgeIntensity += smoothstep(float(radius), 0.0, distance);
            }
        }
    }

    edgeIntensity = clamp(edgeIntensity, 0.0, 1.0);

    vec4 baseColor = texture(screenTexture, TexCoords);
    vec3 color = vec3(0.6314, 0.3765, 0.0863);
    vec4 edgeColor = vec4(color, edgeIntensity); 

    FragColor = mix(baseColor, edgeColor, edgeIntensity);
}
