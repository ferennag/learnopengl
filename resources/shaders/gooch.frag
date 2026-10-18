#version 460 core

layout(std140) uniform Camera {
  mat4 Projection;
  mat4 View;
  vec3 CameraPosition;
};

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vNormal;

layout(location = 0) out vec4 fColor;

void main() {
  vec3 L = normalize(vec3(0, 300, 300) - vPosition);
  vec3 V = normalize(CameraPosition - vPosition);
  vec3 N = vNormal;

  vec3 color = vec3(0.2, 0.2, 0.8);
  vec3 cool = vec3(0, 0, 0.55) + 0.25 * color;
  vec3 warm = vec3(0.3, 0.3, 0) + 0.25 * color;
  vec3 highlight = vec3(1,1,1);

  float t = (dot(N, L) + 1) / 2.0;
  vec3 r = 2 * dot(N, L) * N - L;
  vec3 s = clamp((100 * r * V - 97), 0, 1);

  fColor = vec4(s * highlight + (1 - s) * (t * warm + (1-t) * cool), 1.0);
}
