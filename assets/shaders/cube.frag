#version 460 core

layout(location = 0) in vec3 VertexColor;

out vec4 FragColor;

void main() {
  FragColor = vec4(VertexColor, 1.0);
}
