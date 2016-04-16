#version 330 core

in vec2 uv;

uniform float tex_width;
uniform float tex_height;

out vec3 color;

float permutation(float pos){return p[pos];}

float fade(float t){return t * t * t * (t * (t * 6 - 15) + 10);}

vec2 grad(int index){
   index = index % 8;
   return gradients[index];}

uniform int p[512] = int[](151,160,137,91,90,15,
   131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
   190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
   88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
   77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
   102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
   135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
   5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
   223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
   129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
   251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
   49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
   138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180,151,160,137,91,90,15,
   131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
   190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
   88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
   77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
   102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
   135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
   5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
   223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
   129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
   251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
   49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
   138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180);

   uniform vec2 gradients[8] = {vec2(0.0,1.0),
                        vec2(1.0,1.0),
                        vec2(1.0,0.0),
                        vec2(1.0,-1.0),
                        vec2(0.0,-1.0),
                        vec2(-1.0,-1.0),
                        vec2(-1.0,0.0),
                        vec2(-1.0,1.0)
                        };

void main() {

   float N = 4; // number of tiles in each dimension

   vec2 grads[4];
   vec2 difference_vectors[4];
   vec2 dot_products[4];

   // 1.1 check which tile the coordinate is inside
   float norm_x = normalize(uv.x);
   float norm_y = normalize(uv.y);
   norm_x = norm_x/tex_width;
   norm_y = norm_y/tex_height;

   float tile_width = tex_width/N;
   float tile_height = tex_height/N;

   // 1.2 for all corners, calculate the gradients
   float index_x = norm_x * 255;
   float index_y = norm_y * 255;

   vec2 corners[4] = {vec2(0.0,0.0),
                  vec2(0.0,1.0),
                  vec2(1.0,1.0),
                  vec2(1.0,0.0)};
   }

   int random_int = 0;

   vec2 bl_corner = vec2(floor(norm_x/tile_width),floor(norm_y/tile_height);
   

   vec2 current_corner;
   // for every corner, calculate gradient vector
   for(int i = 0; i<4; i++){
      current_corner = bl_corner + corners[i];

      random_int = permutation(current_corner.x);
      random_int = permutation(current_corner.y + random_int);

      grads[i] = grad(random_int);
   
      // 1.3 calulate distance vector from corners to point. 
      difference_vectors[i] = vec2(norm_x,norm_y) - current_corner;           

      dot_products[i] = dot(normalize(difference_vectors[i]),normalize(grads[i]));
   }
   // 1.4 do the mixing and fading stuff

   float fx = fade(norm_x);
   float fy = fade(norm_y);

   float st = mix(dot_products[0],dot_products[1],fx);
   float uv = mix(dot_products[2],dot_products[3],fx);

   float noise = mix(st,uv,fy);

   color = noise;
}

