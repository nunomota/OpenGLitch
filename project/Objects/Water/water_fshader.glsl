#version 330

in vec2 uv;
in vec4 clipSpace;

out vec3 color;

uniform sampler2D tex0;
uniform sampler2D tex1;

void main() {
    vec2 ndc = (clipSpace.xz/clipSpace.w)/2.0f + 0.5f;

    vec2 reflectTexCoords = vec2(ndc.x, -ndc.y);

    //color = mix(texture(tex0, uv).rgb, texture(tex1, vec2(_u, _v)).rgb, vec3(0.15));
    color = mix(texture(tex1, reflectTexCoords).rgb, texture(tex0, uv).rgb, vec3(0.5));
}
