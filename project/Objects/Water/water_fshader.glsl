#version 330

in vec2 uv;
in vec4 gl_FragCoord;
in vec4 pos_3d;

out vec3 color;

uniform sampler2D tex0;
uniform sampler2D tex1;

float waveStrength = 0.05f;

void main() {
    ivec2 window_size = textureSize(tex1, 0);

    //float _u = (gl_FragCoord.x - 0.5f) / window_size.x;
    //float _v = (gl_FragCoord.y - 0.5f) / window_size.y;

    float _u = ((pos_3d.x/pos_3d.w + 1.0f)/2.0f);
    float _v = ((pos_3d.y/pos_3d.w + 1.0f)/2.0f);

    vec2 distortion = (texture(tex0, uv).rg * 2.0f - 1.0f) * waveStrength;

    color = mix(texture(tex0, uv).rgb, texture(tex1, vec2(_u, _v)).rgb, vec3(0.15));
    color = mix(texture(tex1, vec2(_u, 1.0f-_v) + distortion).rgb, vec3(0.0f, 0.0f, 1.0f), vec3(0.15));
    //color = texture(tex0, uv).rgb;
}
