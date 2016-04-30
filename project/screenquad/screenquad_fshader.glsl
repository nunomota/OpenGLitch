#version 330 core
#define M_PI 3.14

in vec2 uv;
uniform float tex_width;
uniform float tex_height;
uniform float num_tiles;
uniform float H;
uniform float gain;
uniform float offset;
uniform int octaves;
uniform sampler1D texture_1d_id_;
out vec3 color;

int p[512] = int[](151,160,137,91,90,15,
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

// gradient vectors
vec2 g[8] = vec2[](vec2(0.0,1.0),
                   vec2(1.0,1.0),
                   vec2(1.0,0.0),
                   vec2(1.0,-1.0),
                   vec2(0.0,-1.0),
                   vec2(-1.0,-1.0),
                   vec2(-1.0,0.0),
                   vec2(-1.0,1.0)
                   );


int PERMUTATION(int pos){pos = pos % 512; return p[pos];}

float FADE(float t){return t * t * t * (t * (t * 6.0 - 15.0) + 10.0);}
//float FADE(float t){return (6 * t * t * t * t * t) - (15.0 * t * t * t * t) + (10.0 * t * t * t);}

float TEX_1D(float coord){return float(texture(texture_1d_id_, coord));}

//float FADE(float t){return t * t * (3.0 - 2.0 * t);} // classic perlin noise curve
vec2 GRAD(int index){
   index = index % 8;
   return g[index];}

/*vec2 GRAD(int index){
   return g[1];
 }
*/

float MIXER(float x, float y, float alpha){return (1.0-alpha) * x + alpha * y;}

float PERLIN_NOISE(vec2 uv){

  float num_of_tiles = num_tiles;
  // 1.1 check which tile the coordinate is inside
  float x = uv.x * num_of_tiles;
  float y = uv.y * num_of_tiles;

  // 1.2 for all corners, calculate the gradients
  // x and y component for bottom left corner
  float bl_x = floor(x);
  float bl_y = floor(y);
  vec2 bl_corner = vec2(bl_x, bl_y);   
  
  // tile corners
  vec2 a_corner = bl_corner;
  vec2 b_corner = bl_corner + vec2(1.0,0.0);
  vec2 c_corner = bl_corner + vec2(0.0,1.0);
  vec2 d_corner = bl_corner + vec2(1.0,1.0);

  // difference vector
  vec2 a = vec2(x,y) - a_corner;
  vec2 b = vec2(x,y) - b_corner;
  vec2 c = vec2(x,y) - c_corner;
  vec2 d = vec2(x,y) - d_corner;

  // get random integer from by converting the coordinates to a 1d array. 
  int ra = PERMUTATION(int(a_corner.x + (num_of_tiles * a_corner.y) * 512 / (num_of_tiles * num_of_tiles)));
  int rb = PERMUTATION(int(b_corner.x + (num_of_tiles * b_corner.y) * 512 / (num_of_tiles * num_of_tiles)));
  int rc = PERMUTATION(int(c_corner.x + (num_of_tiles * c_corner.y) * 512 / (num_of_tiles * num_of_tiles)));
  int rd = PERMUTATION(int(d_corner.x + (num_of_tiles * d_corner.y) * 512 / (num_of_tiles * num_of_tiles)));

  // angles
  float ga_y = TEX_1D(ra / 255.0);
  float gb_y = TEX_1D(rb / 255.0);
  float gc_y = TEX_1D(rc / 255.0);
  float gd_y = TEX_1D(rd / 255.0);
 
  vec2 ga = vec2(sin(ga_y), cos(ga_y));
  vec2 gb = vec2(sin(gb_y), cos(gb_y));
  vec2 gc = vec2(sin(gc_y), cos(gc_y));
  vec2 gd = vec2(sin(gd_y), cos(gd_y)); 

  vec2 gr = vec2(1.0,1.0);
  float s = dot(a, normalize(ga));
  float t = dot(b, normalize(gb));
  float u = dot(c, normalize(gc));
  float v = dot(d, normalize(gd));

  // 1.4 do the mixing and fading (interpolation)
  float fx = FADE(fract(x));
  float fy = FADE(fract(y));

  float s_t = MIXER(s,t,fx);
  float u_v = MIXER(u,v,fx);
  float noise = MIXER(s_t,u_v,fy);

  return noise;
}

void main() {

/*

  float noise = PERLIN_NOISE(uv);
  int num_octaves = octaves;

   // fBm
   float h = H;
   float total = 0.0;
   //float freq = 2.0; // start with small frequence
   //float gain = 0.5; //  multiply amp by this after each octave
   float lacunarity = 2.143212; // multiply frequency with this after each octave
   vec2 point = uv;

   for(int i = 0; i<num_octaves; i++){
      total += PERLIN_NOISE(point + 0.0) * pow(lacunarity, -h*i);
      point = point * lacunarity;
   }
   
*/
   
  //inital values
  float h = H;
  float g = gain;
  float frequency = 1.0;
  float result = 0.0;
  float lacunarity = 2.143212; // multiply frequency with this after each octave
  
  vec2 coords = uv;
  // first octave
  float noise = PERLIN_NOISE(coords);

  // absolute value creates ridges
  if (noise < 0.0) noise = -noise;
  noise = offset - noise;

  // square signal to increase sharpness of ridges.
  noise *= noise;
  result = noise;
  float weight = 1.0;

  for( int i = 1; i<octaves; i++ ){
    // use a new point variable
    coords.x *= lacunarity;
    coords.y *= lacunarity;
    weight = noise * g;

    if(weight > 1.0 ) weight = 1.0;
    if(weight < 0.0 ) weight = 0.0;

    noise = PERLIN_NOISE(coords);
    if(noise < 0.0) noise = -noise;
    noise = offset - noise;
    noise *= noise;
    noise *= weight;

    result += noise * pow(frequency, -h);
    frequency *= lacunarity;
  }    

  color = vec3(result,result,result); // without fBM 
  //color = vec3(total,total,total); // without fBM
}