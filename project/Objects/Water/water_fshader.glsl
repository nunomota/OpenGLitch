#version 330

in vec2 uv;
in vec2 displacement_vector;
in vec4 gl_FragCoord;
in vec4 pos_3d;

out vec3 color;

uniform sampler2D tex0;    // DUDV map
uniform sampler2D tex1;    // reflection texture
uniform sampler2D tex2;    // normal map
uniform sampler2D tex3;    // refraction texture
uniform float time;

uniform vec3 lightDirection;
uniform vec3 lightPosition;
uniform vec3 cameraPosition;
uniform vec3 cameraDirection;
uniform vec3 La, Ld, Ls;

float waveStrength = 0.03f;
float shineDumper = 4.0f;
float reflectivity = 0.2f;

float tilling  = 6.0f;
float speed_factor = 1.0f/300.0f;
vec3 water_color = vec3(0.0f, 0.4f, 0.6f);

void main() {

    float _u = ((pos_3d.x/pos_3d.w + 1.0f)/2.0f);
    float _v = ((pos_3d.y/pos_3d.w + 1.0f)/2.0f);

    vec2 distortion = (texture(tex0, (uv + displacement_vector + time*speed_factor)*tilling).rg * 2.0f - 1.0f) * waveStrength;
    vec4 normalMapColor = texture(tex2, (uv + displacement_vector + time*speed_factor)*tilling);
    vec3 normal = vec3(normalMapColor.r * 2.0f - 1.0f, normalMapColor.b, normalMapColor.g * 2.0f - 1.0f);
    normal = normalize(normal);

    vec2 reflectTexCoords = vec2(_u, 1.0f-_v) + distortion;
    vec2 refractTexCoords = vec2(_u, _v) + distortion;

    if (cameraPosition.y >= 0.0f) {
        // specular calculation
        vec3 reflectedLight = reflect(normalize(pos_3d.xyz - lightPosition), normal);
        float specular = max(dot(reflectedLight, normalize(pos_3d.xyz - cameraPosition)), 0.0f);
        specular = pow(specular, shineDumper);
        vec3 specularHighlights = Ls * specular * reflectivity;

        float refractive_factor = dot(normalize(-cameraDirection), vec3(0.0f, 1.0f, 0.0f));
        //refractive_factor = pow(refractive_factor, 2.0f);
        color = mix(texture(tex1, reflectTexCoords).rgb, texture(tex3, refractTexCoords).rgb, refractive_factor);
        
        color = mix(color, water_color, vec3(0.15)) + specularHighlights;
    } else {
        color = mix(texture(tex3, refractTexCoords).rgb, water_color, pow(clamp(gl_FragCoord.z/gl_FragCoord.w, 0.001f, 0.999f), 3.0f));     
    }
}
