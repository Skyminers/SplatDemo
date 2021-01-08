#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 ourColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 color;

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1);
    ourColor = color;
}