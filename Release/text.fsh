#version 330 core

in vec2 texCoords;

out vec4 color;

uniform sampler2D text;
uniform vec3 textColor;

void main(void) {
    vec4 sampled = vec4(1, 1, 1, texture(text, texCoords).r);
    color = vec4(textColor, 1) * sampled;
}
