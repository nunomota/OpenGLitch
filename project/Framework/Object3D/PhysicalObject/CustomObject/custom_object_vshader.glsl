#version 330

uniform mat4 MVP;

in vec3 position;
in vec2 v_uv;
in vec3 v_normal;

out vec2 uv;
out vec3 normal;

void main() {
    gl_Position = MVP * vec4(position, 1.0);

    uv = v_uv;
    normal = v_normal;
}
