#version 330
const vec3 CAMERA_POSITION = vec3(4,4,2);
in vec2 uv;

out vec3 color;

//yellow for the bottom part of the terrain
vec3 Ld = vec3(0.9f, 0.9f, 0.0f);
vec3 kd = vec3(0.9f, 0.9f, 0.0f);

uniform sampler2D tex0;

int iterations = 15;
vec3 normal_mv = vec3 (0.0f,0.0f,0.0f);

void main() {
    int max_i = iterations * 2 + 1;
    int max_j = max_i;
    ivec2 tex_size = ivec2(textureSize(tex0, 0));
    //vec2 bl_corner = vec2(uv.x - (1/tex_size.x), uv.y - (1/tex_size.y));
    vec2 bl_corner = uv - (iterations *(1/tex_size));
    //looping to get the neighbors coordinates ((x-z, y+z), (x+z, y+z) and so on...
    for (int i = 0; i<= max_i; i++){
        for(int j = 0; j<= max_j; j++){
            vec2 neighbor_2D = bl_corner + vec2(i*(1/tex_size.x), j*(1/tex_size.y));

            //getting the neighbors height
            float height = texture(tex0, neighbor_2D).x;

            //getting the position of the neighbor
            vec3 neighbor_3D = vec3(neighbor_2D.x, neighbor_2D.y, height);
            normal_mv += neighbor_3D;
        }
    }

    normal_mv = normal_mv/(max_i*max_j);
    //normal_mv = vec3(uv.x, uv.y, texture(tex0, uv).x);
    vec3 n = normalize(normal_mv);
    n = vec3(n.x,n.y,n.z);

    vec3 cam = normalize(CAMERA_POSITION);
    vec3 light_dir = (n - cam);
    //vec3 light_dir = (n - CAMERA_POSITION);
    vec3 l = normalize(light_dir);

    //Now we do the difuse shading
    float temp;
    float nl = ((temp = dot(n,l)) < 0)             ? 0.0f : temp;
    if(n.z > 0.10f){
         //white
        Ld = vec3(1.0f, 1.0f, 1.0f);
        kd = vec3(1.0f, 1.0f, 1.0f);
    }else if(n.z > 0.01f){
        //yellow
        Ld = vec3(0.0f, 1.0f, 0.0f);
        kd = vec3(0.0f, 1.0f, 0.0f);
    }
    vec3 diffuse = kd * nl * Ld;

    color = diffuse.xyz;
}
