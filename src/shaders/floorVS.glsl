#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in float aAlpha;
layout (location = 3) in vec2 aNoiseTexCoord;

out vec3 ourColor;
out vec3 normal;
out vec3 worldPos;
out float ourAlpha;
out vec2 NoiseTexCoord;
out vec4 fragPosLightSpace;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;

void main() {
    gl_Position = model * vec4(aPos, 1);
    worldPos = vec3(gl_Position);
    fragPosLightSpace = lightSpaceMatrix * vec4(worldPos, 1.0);

    normal = vec3(0,1,0);
    gl_Position = projection * view * gl_Position;
    ourColor = aColor;
    ourAlpha = aAlpha;
    NoiseTexCoord = aNoiseTexCoord;
}
