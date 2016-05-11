#version 330
#define M_PI 3.14f

in vec3 position;
uniform sampler2D tex0;

out vec2 uv;
out vec3 pos_3d;

uniform mat4 MVP;

void main() {
    uv = (vec2(position.x, -position.z) + vec2(1.0, 1.0)) * 0.5;

    //Find displacement of vertex
    float height = texture(tex_height, uv).x;

    int iterations = 15;

    int max_i = iterations * 2 + 1;
    int max_j = max_i;
    vec2 tex_size = vec2(textureSize(tex_height, 0));

    vec2 bl_corner = uv - (iterations * (1/tex_size));

    float average_height = texture(tex_height, uv).x;

    for (int i = 0; i<= max_i; i++){
        for(int j = 0; j<= max_j; j++){
            vec2 neighbor_2D = bl_corner + vec2(i*(1/tex_size.x), j*(1/tex_size.y));

            //getting the neighbors height
            average_height += texture(tex_height, neighbor_2D).x;
        }
    }
    average_height /= max_i*max_j;

    pos_3d = vec3(position.x, average_height, -position.y);

    gl_Position = MVP * vec4(pos_3d, 1.0);
}