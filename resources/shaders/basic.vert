#version 460 core

uniform mat4 Projection;
uniform mat4 View;

layout(location = 0) in vec2 vPosition;

void main() {
  gl_Position = Projection * View * vec4(vPosition, 0.0, 1.0);
}
