#version 330

in vec2 uv;

out vec3 color;

uniform sampler2D tex0;

void main() {
    color = texture(tex0, uv).rgb;
}
