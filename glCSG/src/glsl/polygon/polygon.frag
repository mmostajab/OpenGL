#version 450 core

layout(location = 0) out vec4 color;

in VS_OUT {

	vec3 normal;
	float sign;

} fs_in;

void main(){
	color = vec4(fs_in.normal, 1.0f);
}