#version 330
vec3 CAMERA_POSITION = (4,4,2);
in vec2 uv;

out vec3 color;

uniform sampler2D height_tex;
float iterations = 2;


void main() {
    vec3 sum_vec(0,0,0);
    //looping to get the neighbors coordinates ((x-z, y+z), (x+z, y+z) and so on...
    for (int i = 0; i< iterations * 2 + 1; i++){
        for(int j = 0; i< iterations * 2 + 1; j++){
            vec2 neighbor_2D = (uv.x - iterations + i, uv.y - iterations + uv.y + j);
            //getting the neighbors height
            float height = texture(height_tex, neighbors);
            vec3 neighbor_3D = vec3(neighbor_2D, height);
            sum_vec += neighbor_3D;
        }
    }

    sum_vec = sum_vec/pow(iterations * 2 +1, 2);
    vec3 norm_sum = normalize(sum_vec);

    vec3 cam_uv = (CAMERA_POSITION - uv);

    vec3 norm = cross()


    color = texture(tex, uv).rgb;
}
