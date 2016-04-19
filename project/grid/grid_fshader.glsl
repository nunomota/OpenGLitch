#version 330
const vec3 CAMERA_POSITION = vec3(4,4,2);
in vec2 uv;

out vec3 color;

vec3 Ld = vec3(1.0f, 1.0f, 1.0f);
vec3 kd = vec3(0.9f, 0.5f, 0.5f);
uniform sampler2D height_tex;

int iterations = 1;
vec3 normal_mv = vec3 (0.0f,0.0f,0.0f);

void main() {
    int max_i = iterations * 2 + 1;
    int max_j = max_i;
    vec2 bl_corner = vec2(uv.x - float(iterations), uv.y - float(iterations));
    //looping to get the neighbors coordinates ((x-z, y+z), (x+z, y+z) and so on...
    for (int i = 0; i<= max_i; i++){
        for(int j = 0; i<= max_j; j++){
            vec2 neighbor_2D = bl_corner + vec2(float(i), float(j));
            //getting the neighbors height
            float height = texture(height_tex, neighbor_2D).x;

            //getting the position of the neighbor
            vec3 neighbor_3D = vec3(neighbor_2D.x, neighbor_2D.y, height);
            normal_mv += neighbor_3D;
        }
    }


    normal_mv = normal_mv/(max_i*max_j);

    //normal_mv = vec3(uv.x, uv.y, texture(height_tex, uv).x);
    vec3 n = normalize(normal_mv);

    vec3 light_dir = (n - CAMERA_POSITION);
    vec3 l = normalize(light_dir);



    //Now we do the difuse shading
    float temp;
    float nl = ((temp = dot(n,l)) < 0)             ? 0.0f : temp;
    vec3 diffuse = kd * nl * Ld;

    color = (diffuse).xyz;
    /*
    // # P.xy store the position for which we want to calculate the normals
      // # height() here is a function that return the height at a point in the terrain

      // read neightbor heights using an arbitrary small offset
      vec3 off = vec3(1.0, 1.0, 0.0);
      float hL = texture(height_tex, uv.xy - off.xz).x;
      float hR = texture(height_tex, uv.xy + off.xz).x;
      float hD = texture(height_tex, uv.xy - off.zy).x;
      float hU = texture(height_tex, uv.xy + off.zy).x;
      vec3 N;

      // deduce terrain normal
      N.x = hL - hR;
      N.y = hD - hU;
      N.z = 2.0;
      N = normalize(N);
    */
}
