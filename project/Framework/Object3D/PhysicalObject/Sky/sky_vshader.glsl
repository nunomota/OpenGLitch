#version 330

uniform mat4 MVP;

in vec3 position;
out vec3 TexCoords;

void main() {
    gl_Position = MVP * vec4(position, 1.0);
    TexCoords = position;
}
