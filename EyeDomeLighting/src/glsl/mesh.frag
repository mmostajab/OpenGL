#version 430 core

layout(location = 0) uniform vec3 lightDir = vec3(1.0f);

in VS_OUT {
	vec3 viewNormal;
} fs_in;

layout(location = 0) out vec4 out_color;

void main() {
	float diffuse = dot(lightDir, fs_in.viewNormal);
	out_color = vec4(diffuse * vec3(0.2f, 0.8f, 0.5f), 1.0f);
    //out_color =  vec4((fs_in.viewNormal + vec3(1.0f)) / 2.0f, 1.0f);
}