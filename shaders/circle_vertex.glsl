#version 330 core
layout (location = 0) in vec3 aPos;

uniform vec2 circlePos;
uniform vec2 iResolution;

void main()
{
    vec3 pos = aPos;
    pos.x /= iResolution.x / iResolution.y;
    pos.xy += circlePos;
    gl_Position = vec4(pos, 1.0);
}