#version 460 core

layout(std140) uniform Camera {
  mat4 Projection;
  mat4 View;
};

uniform mat4 Model;

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vNormal;

layout(location = 0) out vec3 normal;

void main() {
  gl_Position = Projection * View * Model * vec4(vPosition, 1.0);
  normal = normalize(mat3(Model) * vNormal);
}
