#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform sampler2D maskTexture;

void main()
{
    // Отримуємо розмір текстури
    vec2 texSize = textureSize(maskTexture, 0);
    vec2 texelSize = 1.0 / texSize; // Розмір одного пікселя

    // Отримуємо значення маски поточного пікселя
    float center = texture(maskTexture, TexCoords).r;

    // Радіус перевірки (половина розміру лінії)
    int radius = 4;
    float edgeIntensity = 0.0;

    // Перевірка сусідніх пікселів
    for (int y = -radius; y <= radius; y++) {
        for (int x = -radius; x <= radius; x++) {
            if (x == 0 && y == 0) continue; // Пропускаємо поточний піксель

            // Обчислюємо координати сусіднього пікселя
            vec2 offset = vec2(x, y) * texelSize;
            vec2 neighborCoords = TexCoords + offset;

            // Перевіряємо межі текстури
            if (neighborCoords.x < 0.0 || neighborCoords.x > 1.0 || 
                neighborCoords.y < 0.0 || neighborCoords.y > 1.0) 
            {
                continue;
            }

            // Отримуємо значення сусіднього пікселя
            float neighbor = texture(maskTexture, neighborCoords).r;

            // Якщо значення відрізняється, збільшуємо інтенсивність обводки
            if (neighbor != center) {
                float distance = length(vec2(x, y));
                edgeIntensity += smoothstep(float(radius), 0.0, distance);
            }
        }
    }

    // Нормалізація інтенсивності обводки
    edgeIntensity = clamp(edgeIntensity, 0.0, 1.0);

    // Об'єднання обводки з текстурою
    vec4 baseColor = texture(screenTexture, TexCoords);
    vec3 color = vec3(0.6314, 0.3765, 0.0863); // Основний колір
    vec4 edgeColor = vec4(color, edgeIntensity); // Червона обводка

    FragColor = mix(baseColor, edgeColor, edgeIntensity);
}
