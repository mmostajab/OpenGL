#version 430 core

layout(location = 0) uniform vec3 lightDir = vec3(1.0f);

in VS_OUT {
	vec4 worldCoord;
	vec3 viewNormal;
} fs_in;

layout(location = 0) out vec4 out_color;
layout(location = 1) out vec4 out_world_coord;

//#define DIFFUSE_LIGHTING

void main() {
	
	vec3 color = vec3(0.2f, 0.8f, 0.5f);
#ifdef DIFFUSE_LIGHTING
	float diffuse = dot(lightDir, fs_in.viewNormal);
	out_color = vec4(diffuse * color, 1.0f);
#else
	out_color = vec4(color, 1.0f);
#endif

	out_world_coord = fs_in.worldCoord;
    //out_color =  vec4((fs_in.viewNormal + vec3(1.0f)) / 2.0f, 1.0f);
}