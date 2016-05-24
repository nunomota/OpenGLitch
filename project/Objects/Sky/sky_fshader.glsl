#version 330

in vec3 TexCoords;
out vec3 color;

uniform samplerCube tex;

void main() {
    color = texture(tex, TexCoords).rgb;
}