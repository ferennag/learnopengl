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
    vec3 surface = vec3(0.1f, 0.1f, 0.1f);
    vec3 cool = vec3(0.0f, 0.0f, 0.25f) + 0.25f * surface;
    vec3 warm = vec3(0.3f, 0.3f, 0.0f) + 0.25f * surface;
    vec3 highlight = vec3(1.0f, 1.0f, 1.0f);

    vec3 n = normalize(normal);
    vec3 v = normalize(eye - worldPos); // eye vector from vertex
    vec3 l = normalize(light.position - worldPos); // light vector from vertex
    float t = (dot(n, l) + 1.0f) / 2.0f; // Calculates the angle between the light and the normal
    vec3 r = reflect(-l, n); // Calculates the vector of the reflected light
    float s = clamp((100 * dot(r, v) - 97), 0.0f, 1.0f); // Calculates the amount of highlight (based on eye vector and reflected light angle)

    // mix() interpolates linearly between 2 colors, using t as weight (small t cooler color, bigger t means warmer color)
    vec3 tone = mix(cool, warm, t);
    vec3 color = mix(tone, highlight, s);

    FragColor = vec4(color, 1.0f);
}
