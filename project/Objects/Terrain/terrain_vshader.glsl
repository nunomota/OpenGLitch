#version 330
#define M_PI 3.14f

in vec3 position;
uniform sampler2D tex0;
out vec2 uv;
out vec3 pos_3d;
out float height;
uniform mat4 MVP;
uniform vec3 cameraPosition;
uniform vec4 CLIP_PLANE;

out vec2 displacement_vector;

void main() {
    uv = (vec2(position.x, -position.z) + vec2(2.0, 2.0)) * 0.25;
    displacement_vector = vec2(cameraPosition.x, -cameraPosition.z);
    height = texture(tex0, uv + displacement_vector).x * 2.0f - 1.0f;
    
    // TODO pass in the plane's normal, multiply by the height and then add to position vector.
    pos_3d = vec3(position.x, position.y+height, position.z);

    gl_ClipDistance[0] = dot(vec4(pos_3d, 1.0), CLIP_PLANE);

    gl_Position = MVP * vec4(pos_3d, 1.0);
}