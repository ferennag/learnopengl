#version 460 core

layout(location = 0) in vec3 inColor;
layout(location = 1) in vec2 inTexCoords;

out vec4 FragColor;

uniform sampler2D tex;

void main() {
    FragColor = texture(tex, inTexCoords);
}
