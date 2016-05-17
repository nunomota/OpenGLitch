#version 330

in vec2 uv;
in vec4 gl_FragCoord;

out vec3 color;

uniform sampler2D tex0;
uniform sampler2D tex1;

void main() {
    ivec2 window_size = textureSize(tex1, 0);

    float _u = (gl_FragCoord.x - 0.5f) / window_size.x;
    float _v = (gl_FragCoord.y - 0.5f) / window_size.y;

    color = mix(texture(tex0, uv).rgb, texture(tex1, vec2(_u, _v)).rgb, vec3(0.15));
    color = texture(tex0, uv).rgb;
}
