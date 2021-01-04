#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 texCoord;
out vec3 normal;
out vec3 worldPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = model * vec4(aPos, 1);
    worldPos = vec3(gl_Position);

    vec4 centerPosition = model * vec4(vec3(.0,.0,.0), 1);

    normal = vec3(gl_Position - centerPosition);
    gl_Position = projection * view * gl_Position;
    texCoord = aTexCoord;
}
