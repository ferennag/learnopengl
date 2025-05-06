#version 460 core

uniform mat4 mvp;

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoords;

layout(location = 0) out vec3 outColor;
layout(location = 1) out vec2 outTexCoords;

void main() {
    gl_Position = mvp * vec4(inPos, 1.0);
    outColor = inColor;
    outTexCoords = inTexCoords;
}
