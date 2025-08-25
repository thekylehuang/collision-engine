#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 instancePos;

uniform float aspectRatio;

void main() {
    vec3 scaledPos = aPos;
    scaledPos.x /= aspectRatio;
    gl_Position = vec4(scaledPos.xy + instancePos, 0.0, 1.0);
}
