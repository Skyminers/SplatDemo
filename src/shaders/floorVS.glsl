#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 ourColor;
out vec3 normal;
out vec3 worldPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = model * vec4(aPos, 1);
    worldPos = vec3(gl_Position);

    normal = vec3(0,1,0);
    gl_Position = projection * view * gl_Position;
    ourColor = aColor;
}
