#version 330

in vec2 uv;

out vec3 color;

uniform sampler2D tex0;
uniform sampler2D tex1;

void main() {
    vec4 main_texture = texture(tex0, uv);
    vec4 reflection = texture(tex1, uv);
    color = mix(main_texture, reflection, main_texture.a).rgb;
    color = texture(tex1, uv).rgb;
}
