#version 460 core

layout(std140) uniform Camera {
  mat4 Projection;
  mat4 View;
  vec3 CameraPosition;
};

uniform mat4 Model;
uniform mat3 Normal;

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vNormal;

layout(location = 0) out vec3 fPosition;
layout(location = 1) out vec3 fNormal;

void main() {
  vec4 position = Model * vec4(vPosition, 1.0);
  gl_Position = Projection * View * position;
  fPosition = position.xyz;
  fNormal = normalize(Normal * vNormal);
}
