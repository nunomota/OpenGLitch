#version 330

in vec2 uv;
in vec3 pos_3d;
in float height;
uniform sampler2D tex0; // perlin texture
uniform sampler2D tex1; // perlin normalmap

uniform vec3 lightDirection;
uniform vec3 lightPosition;
uniform vec3 cameraPosition;
uniform vec3 La, Ld, Ls;
uniform vec3 Ma, Md, Ms;

out vec3 color;

void main() {

    vec3 height_color;
    float underwater_fix = 1.0f;

    // normal caculation according to normalmap
    vec4 normalMapColor = texture(tex1, uv);
    vec3 n = vec3(normalMapColor.r * 2.0f - 1.0f, normalMapColor.b, normalMapColor.g * 2.0f - 1.0f);

    vec3 l = normalize(lightDirection);

    //Now we do the difuse shading
    float temp;
    float nl = ((temp = dot(n,l)) < 0) ? 0.0f : temp;

    if (height > 0.66f) {
        //white
        height_color = vec3(1.0f, 1.0f, 1.0f);
    } else if (height > 0.33f) {
        //white
        height_color = vec3(0.7f, 0.7f, 0.7f);
    } else if(height > 0.0f){
        //green
        height_color = vec3(0.0f, 1.0f, 0.0f);
    } else {
        //yellow
        height_color = vec3(0.9f, 0.9f, 0.0f);
        underwater_fix = 1.0f-gl_FragCoord.z;
    }

    vec3 diffuse = Md * nl * Ld * height_color * underwater_fix;
    color = diffuse.xyz;
}
