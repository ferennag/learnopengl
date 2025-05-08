#version 460 core

struct PointLight {
    vec3 position;
    vec3 color;
};

uniform PointLight light;
uniform vec3 eye;

layout(location = 0) in vec3 worldPos;
layout(location = 1) in vec3 normal;

out vec4 FragColor;

void main() {
    vec3 surface = vec3(0.8f, 0.5f, 0.2f);
    vec3 cool = vec3(0.0f, 0.0f, 0.55f) + 0.25f * surface;
    vec3 warm = vec3(0.3f, 0.3f, 0.0f) + 0.25f * surface;
    vec3 highlight = vec3(1.0f, 1.0f, 1.0f);

    vec3 v = normalize(eye - worldPos);
    vec3 l = normalize(light.position - worldPos);
    float t = (dot(normal, l) + 1.0f) / 2.0f;
    vec3 r = 2 * dot(normal, l) * normal - l;
    float s = clamp((100 * dot(r, v) - 97), 0.0f, 1.0f);
    vec3 shaded = s * highlight + (1 - s) * (t * warm + (1 - t) * cool);

    FragColor = vec4(shaded, 1.0f);
}
