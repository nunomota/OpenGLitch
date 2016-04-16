#version 330
#define M_PI 3.14f

in vec2 position;
uniform sampler2D tex_height;

out vec2 uv;

uniform mat4 MVP;

void main() {
    uv = (position + vec2(1.0, 1.0)) * 0.5;

    vec3 pos_3d = vec3(position.x, 0, -position.y);

    gl_Position = MVP * vec4(pos_3d, 1.0);
}
