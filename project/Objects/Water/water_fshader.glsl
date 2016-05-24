#version 330

in vec2 uv;
in vec2 displacement_vector;
in vec4 gl_FragCoord;
in vec4 pos_3d;

out vec3 color;

uniform sampler2D tex0;    // DUDV map
uniform sampler2D tex1;    // reflection texture
uniform sampler2D tex2;    // normal map
uniform float time;

uniform vec3 lightDirection;
uniform vec3 lightPosition;
uniform vec3 cameraPosition;
uniform vec3 La, Ld, Ls;

float waveStrength = 0.1f;
float shineDumper = 20.0f;
float reflectivity = 0.6;

void main() {

    float _u = ((pos_3d.x/pos_3d.w + 1.0f)/2.0f);
    float _v = ((pos_3d.y/pos_3d.w + 1.0f)/2.0f);

    vec2 distortion = (texture(tex0, uv + displacement_vector + time/200.0f).rg * 2.0f - 1.0f) * waveStrength;
    vec4 normalMapColor = texture(tex2, uv + displacement_vector + time/200.0f);
    vec3 normal = vec3(normalMapColor.r * 2.0f - 1.0f, normalMapColor.b, normalMapColor.g * 2.0f - 1.0f);
    normal = normalize(normal);

    vec2 reflectTexCoords = vec2(_u, 1.0f-_v) + distortion;

    // specular calculation
    vec3 reflectedLight = reflect(normalize(pos_3d.xyz - lightPosition), normal);
    float specular = max(dot(reflectedLight, pos_3d.xyz - cameraPosition), 0.0f);
    specular = pow(specular, shineDumper);
    vec3 specularHighlights = Ls * specular * reflectivity;

    color = mix(texture(tex1, reflectTexCoords).rgb, vec3(0.0f, 0.3f, 0.8f), vec3(0.15)) + specularHighlights;
}
