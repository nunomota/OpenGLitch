#version 330
#define M_PI 3.14f

in vec3 position;
uniform sampler2D tex0;

out vec2 uv;
out vec3 pos_3d;

uniform mat4 MVP;

void main() {
    uv = (vec2(position.x, -position.z) + vec2(1.0, 1.0)) * 0.5;

    float height = texture(tex0, uv).x;

    pos_3d = vec3(position.x, -height, position.z);

    gl_Position = MVP * vec4(position, 1.0);
}