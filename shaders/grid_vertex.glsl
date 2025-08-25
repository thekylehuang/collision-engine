#version 330 core
layout (location = 0) in vec3 aPos;

void main() {
    vec3 pos = aPos;
    gl_Position = vec4(pos, 1.0);
}