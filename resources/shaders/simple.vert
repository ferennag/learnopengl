#version 460 core

uniform mat4 model;
uniform mat3 inverseTransposeModel;
uniform mat4 viewProj;

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inNormal;

layout(location = 0) out vec3 worldPos;
layout(location = 1) out vec3 outNormal;

void main() {
    gl_Position = viewProj * model * vec4(inPos, 1.0);
    worldPos = vec3(model * vec4(inPos, 1.0f));
    outNormal = normalize(inverseTransposeModel * inNormal);
}
