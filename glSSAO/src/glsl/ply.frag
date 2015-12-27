#version 430 core

layout(std140, binding = 3) uniform GeneralBlock {
    float color_multiplier;
	vec4 color_background;
} ;

in VS_OUT {
	vec3 view_dir_view;
	vec3 light_dir_view;
	vec3 normal_view;
	vec4 color;
} fs_in;

layout (location = 0) out vec4 out_color;
layout (location = 1) out vec4 normal_depth;

uniform vec3 diffuse_albedo  = vec3(1.0f, 1.0f, 1.0f);
uniform vec3 specular_albedo = vec3(0.7f);
uniform float specular_power = 128.0f; 

uniform float shading_level = 1.0;

void main() {
	vec3 v = normalize(fs_in.view_dir_view);
	vec3 l = normalize(fs_in.light_dir_view);
	vec3 n = normalize(fs_in.normal_view);
	vec3 r = reflect(-l, n);
	vec3 h = normalize(l + v);
	
	vec3 ambient_factor  = vec3(0.20f, 0.20f, 0.20f);
	vec3 diffuse_factor  = diffuse_albedo * max(dot(n,l), 0.0f);
	vec3 specular_factor = specular_albedo * pow(max(dot(r, v), 0.0f), specular_power); 

	out_color = color_multiplier * fs_in.color * mix(vec4(0.0), vec4( ambient_factor + diffuse_factor + specular_factor, 1.0), shading_level);
	normal_depth = vec4(n, fs_in.view_dir_view.z);
}