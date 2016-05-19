#version 330

in vec2 uv;
in vec4 gl_FragCoord;
in vec4 pos_3d;

out vec3 color;

uniform sampler2D tex0;
uniform sampler2D tex1;
uniform float time;

float waveStrength = 0.1f;

void main() {

    float _u = ((pos_3d.x/pos_3d.w + 1.0f)/2.0f);
    float _v = ((pos_3d.y/pos_3d.w + 1.0f)/2.0f);

    vec2 distortion = (texture(tex0, uv + time/200.0f).rg * 2.0f - 1.0f) * waveStrength;

    vec2 reflectTexCoords = vec2(_u, 1.0f-_v) + distortion;

    color = mix(texture(tex1, reflectTexCoords).rgb, vec3(0.0f, 0.0f, 1.0f), vec3(0.15));
}
