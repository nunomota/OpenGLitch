#version 330

uniform mat4 MVP;
uniform vec3 cameraPosition;

in vec3 position;

out vec2 uv;
out vec2 displacement_vector;
out vec4 pos_3d;

void main() {
    uv = (vec2(position.x, -position.z) + vec2(1.0, 1.0)) * 0.5;
    displacement_vector = vec2(cameraPosition.x, -cameraPosition.z);

    pos_3d = MVP * vec4(position, 1.0);

    gl_Position = MVP * vec4(position, 1.0);
}