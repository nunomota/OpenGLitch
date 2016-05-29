#version 330

in vec2 uv;
in vec3 pos_3d;
in float height;
in vec2 displacement_vector;

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
vec3 water_color = vec3(0.0f, 0.4f, 0.6f);

float tilling  = 3.0f;
float speed_factor = 1.0f/50.0f;

void main() {
    
    vec3 height_color;
    vec3 sand;
    vec3 grass;
    vec3 rock;
    vec3 snow;
    vec3 color1;
    vec3 color2;
    vec3 color3;
    vec3 color12;

    //getting the textures
    sand = texture(tex3, uv + displacement_vector).rgb;
    grass = texture(tex4, uv + displacement_vector).rgb;
    rock = texture(tex5, uv + displacement_vector).rgb;
    snow = texture(tex6, uv + displacement_vector).rgb;
    
    //mix all the textures to have the blending
    color1 = mix(sand, grass, height);
    color2 = mix(grass, rock, height);
    color3 = mix(rock, snow, height);
    color12 = mix(color2, color1, height);
    height_color = mix(color12, color3, height);

    // normal caculation according to normalmap
    vec4 normalMapColor = texture(tex1, uv + displacement_vector);
    vec3 n = vec3(normalMapColor.r * 2.0f - 1.0f, normalMapColor.b, normalMapColor.g * 2.0f - 1.0f);

    vec3 l = normalize(lightDirection);

    //Now we do the difuse shading
    float temp;
    float nl = ((temp = dot(n,l)) < 0) ? 0.0f : temp;
    
    if(height < 0.0f){   
        height_color = vec3(0.9f, 0.9f, 0.0f);

        vec4 normalMapColor = texture(tex2, uv*tilling + displacement_vector + time*speed_factor);
        vec3 normal = vec3(normalMapColor.r * 2.0f - 1.0f, normalMapColor.b, normalMapColor.g * 2.0f - 1.0f);
        normal = normalize(normal);

        vec3 reflectedLight = reflect(normalize(lightDirection), normal);
        float specular = max(dot(reflectedLight, normalize(pos_3d.xyz - cameraPosition)), 0.0f);
        specular = pow(specular, shineDumper);
        vec3 specularHighlights = Ls * specular * reflectivity;

        color = mix(height_color, specularHighlights, 0.5f);
        color = mix(color, water_color, gl_FragCoord.z/gl_FragCoord.w);
    } else {
        vec3 ambience = vec3(0.1f, 0.1f, 0.1f) * La;
        vec3 diffuse = Md * nl * Ld;
        color = (ambience + diffuse).xyz;
        color = mix(color, height_color, vec3(0.6f));
    }
}