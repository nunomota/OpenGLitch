#version 330

in vec2 uv;
in vec3 pos_3d;
in float height;

uniform sampler2D tex0; // perlin texture
uniform sampler2D tex1; // perlin normalmap
uniform sampler2D tex2; // water lighting
uniform sampler2D tex3; // sand 
uniform sampler2D tex4; // grass
uniform sampler2D tex5; // rock
uniform sampler2D tex6; // snow


uniform float time;
uniform vec3 lightDirection;
uniform vec3 lightPosition;
uniform vec3 cameraPosition;
uniform vec3 La, Ld, Ls;
uniform vec3 Ma, Md, Ms;

out vec3 color;

float shineDumper = 2.0f;
float reflectivity = 0.6;

void main() {
    
    vec3 height_color;
    vec3 underwater_fix = vec3(0.0f);
    vec3 sand;
    vec3 grass;
    vec3 rock;
    vec3 snow;
    vec3 color1;
    vec3 color2;
    vec3 color3;
    vec3 color12;
    //getting the textures
    sand = texture(tex3, uv).rgb;
    grass = texture(tex4, uv).rgb;
    rock = texture(tex5, uv).rgb;
    snow = texture(tex6, uv).rgb;
    
    //mix all the textures to have the blending
    color1 = mix(sand, grass, height);
    color2 = mix(grass, rock, height);
    color3= mix(rock, snow, height);
    color12 = mix(color2, color1, height);
    height_color = mix(color12, color3, height);
    // normal caculation according to normalmap
    vec4 normalMapColor = texture(tex1, uv);
    vec3 n = vec3(normalMapColor.r * 2.0f - 1.0f, normalMapColor.b, normalMapColor.g * 2.0f - 1.0f);

    vec3 l = normalize(lightDirection);

    //Now we do the difuse shading
    float temp;
    float nl = ((temp = dot(n,l)) < 0) ? 0.0f : temp;
        
    if(height < 0.0f){   
        height_color = vec3(0.9f, 0.9f, 0.0f) * pow((1.0f-gl_FragCoord.z), 0.8f);

        vec4 normalMapColor = texture(tex2, uv + time/200.0f);
        vec3 normal = vec3(normalMapColor.r * 2.0f - 1.0f, normalMapColor.b, normalMapColor.g * 2.0f - 1.0f);
        normal = normalize(normal);

        vec3 reflectedLight = reflect(normalize(pos_3d.xyz - lightPosition), normal);
        float specular = max(dot(reflectedLight, pos_3d.xyz - cameraPosition), 0.0f);
        specular = pow(specular, shineDumper);
        vec3 specularHighlights = Ls * specular * reflectivity;

        underwater_fix = mix(height_color, specularHighlights * (1.0f-gl_FragCoord.z), vec3(0.35));
    }
    vec3 diffuse = Md * nl * Ld * height_color + underwater_fix;
    color = diffuse.xyz;
}