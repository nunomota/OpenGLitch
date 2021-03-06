#version 330

uniform sampler2D tex0;

in vec2 uv;
in vec3 normal;

out vec3 color;

void main() {
    color = texture(tex0, uv).rgb;
}