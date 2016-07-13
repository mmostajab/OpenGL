#version 450 core

layout(location = 0) out vec4 color;

in GS_OUT {

	vec3 normal;
	float sign;

} fs_in;

void main(){
	color = vec4( (fs_in.normal + vec3(1.0f)) / 2.0f, 1.0f );
}