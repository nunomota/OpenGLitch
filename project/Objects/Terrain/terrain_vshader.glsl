#version 330
#define M_PI 3.14f

in vec2 position;
uniform sampler2D tex_height;

out vec2 uv;

uniform mat4 MVP;

void main() {
    uv = (position + vec2(1.0, 1.0)) * 0.5;

    // convert the 2D position into 3D positions that all lay in a horizontal
    // plane.p
    // TODO 6: animate the height of the grid points as a sine function of the
    // 'time' and the position ('uv') within the grid.
    //to be use if we want to animate the water.
    //float height = 0.05 * (sin((position.x + time) * M_PI * 2.0f) * sin((position.y + time) * M_PI * 2.0f));

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

    vec3 pos_3d = vec3(position.x, average_height, -position.y);

    gl_Position = MVP * vec4(pos_3d, 1.0);
}