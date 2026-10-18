#version 460 core

layout(std140) uniform Camera {
  mat4 Projection;
  mat4 View;
};

layout(location = 0) in vec2 vPosition;

void main() {
  gl_Position = Projection * View * vec4(vPosition, 0.0, 1.0);
}
