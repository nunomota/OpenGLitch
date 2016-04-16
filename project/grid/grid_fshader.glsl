#version 330
const vec3 CAMERA_POSITION = vec3(4,4,2);
in vec2 uv;
in vec3 position;

out vec3 color;

uniform vec3 kd, Ld;
uniform sampler2D height_tex;
float iterations = 2;


void main() {
    vec3 sum_vec = vec3 (0,0,0);
    //looping to get the neighbors coordinates ((x-z, y+z), (x+z, y+z) and so on...
    for (int i = 0; i< iterations * 2 + 1; i++){
        for(int j = 0; i< iterations * 2 + 1; j++){
            vec2 neighbor_2D = vec2 (uv.x - iterations + i, uv.y - iterations + uv.y + j);
            //getting the neighbors height
            float height = texture(height_tex, neighbor_2D).x;
            vec3 neighbor_3D = vec3(neighbor_2D, height);
            sum_vec += neighbor_3D;
        }
    }

    sum_vec = sum_vec/pow(iterations * 2 +1, 2);
    vec3 norm_sum = normalize(sum_vec);

    vec3 cam_uv = (CAMERA_POSITION - position);
    vec3 camUV_norm = normalize(cam_uv);
    vec3 normal = cross(camUV_norm, norm_sum);

    //Now we do the difuse shading

    vec3 l = normalize(CAMERA_POSITION);

    float temp;
    float nl = ((temp = dot(normal,l)) < 0)             ? 0.0f : temp;
    vec3 diffuse = kd * nl * Ld;

    color = (diffuse).xyz;

}
