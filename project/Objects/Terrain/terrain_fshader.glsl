#version 330

in vec2 uv;
in vec3 pos_3d;
in float height;
in vec4 shadow_coord;
uniform sampler2D tex0;
uniform sampler2D depthTex;
uniform int PASS;

out vec3 color;

// WARNING: dirty variables should be uniforms
//yellow for the bottom part of the terrain
vec3 Ld = vec3(0.9f, 0.9f, 0.0f);
vec3 kd = vec3(0.9f, 0.9f, 0.0f);
const vec3 CAMERA_POSITION = vec3(4,4,2);

void main() {

    if(PASS == 20){
        float visibility = 1.0;
        if ( texture( depthTex, shadow_coord.xy ).z  <  shadow_coord.z){
           visibility = 0.5;
        }

        vec3 n = normalize(pos_3d);

        vec3 cam = normalize(CAMERA_POSITION);
        vec3 light_dir = (n - cam);
        vec3 l = normalize(light_dir);

        //Now we do the difuse shading
        float temp;
        float nl = ((temp = dot(n,l)) < 0) ? 0.0f : temp;

        if (height > 0.66f) {
            //white
            Ld = vec3(1.0f, 1.0f, 1.0f);
            kd = vec3(1.0f, 1.0f, 1.0f);
        } else if (height > 0.33f) {
           //white
            Ld = vec3(0.7f, 0.7f, 0.7f);
            kd = vec3(0.7f, 0.7f, 0.7f);
        } else if(height > 0.0f){
            //green
            Ld = vec3(0.0f, 1.0f, 0.0f);
            kd = vec3(0.0f, 1.0f, 0.0f);
        }

        vec3 diffuse = kd * nl * Ld;
        diffuse = diffuse * visibility;
        color = diffuse.xyz;

        color = vec3(0.7f,0.7f,0.7f) * visibility;

    }else {
    
        color = vec3(gl_FragCoord.z,gl_FragCoord.z,gl_FragCoord.z);
    
    }


}
