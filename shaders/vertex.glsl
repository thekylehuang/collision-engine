#version 330 core
layout (location = 0) in vec3 aPos;

uniform float aspect;

void main()
{
    vec3 pos = aPos;
    pos.x /= aspect;
    gl_Position = vec4(pos, 1.0);
}