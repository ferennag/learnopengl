#version 460 core

uniform mat4 mvp;

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inNormal;

layout(location = 0) out vec3 outColor;

void main() {
    gl_Position = mvp * vec4(inPos, 1.0);
    outColor = vec3(0.5f, 0.5f, 0.5f);
}
