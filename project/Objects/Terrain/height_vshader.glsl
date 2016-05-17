#version 330 core
in vec3 vpoint;
in vec2 vtexcoord;
out vec2 uv;

void main() {
    gl_Position = vec4(vpoint, 1.0);

    gl_ClipDistance[0] = dot(gl_Position, vec4(0.0f, -1.0f, 0.0f, 0.1f));

    uv = vtexcoord;
}
