#version 330

in vec3 TexCoords;
out vec3 color;

uniform samplerCube tex;

// color buffer
const vec3 COLOR[6] = vec3[](
    vec3(0.0, 0.0, 1.0),
    vec3(0.0, 1.0, 0.0),
    vec3(0.0, 1.0, 1.0),
    vec3(1.0, 0.0, 0.0),
    vec3(1.0, 0.0, 1.0),
    vec3(1.0, 1.0, 0.0));

void main() {
    color = texture(tex, TexCoords).rgb;
}