#version 330

in vec2 uv;

out vec3 color;

uniform int PASS;
uniform sampler2D tex0;

void main() {
	
	// PASS = 0 - normal draw
	if(PASS == 0){
	
		color = texture(tex0, uv).rgb;

	}else{

		float value = gl_FragCoord.z;
		color = vec3(gl_FragCoord.z,gl_FragCoord.z,gl_FragCoord.z);

	}


}
