#version 330

uniform mat4 MVP;
uniform mat4 depthMVP;
uniform int PASS;

in vec3 position;

out vec2 uv;

void main() {
    uv = (vec2(position.x, -position.z) + vec2(1.0, 1.0)) * 0.5;

    if(PASS == 1){
	    gl_Position = depthMVP * vec4(position, 1.0);
    }else{
	    gl_Position = MVP * vec4(position, 1.0);
    }
}