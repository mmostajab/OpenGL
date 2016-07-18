#version 450 core

layout ( location = 0 ) uniform mat4 mvp;

layout(points) in;
layout(triangle_strip, max_vertices=24) out;

in VS_OUT {

	float radius;
	float height;
	float sign;

} gs_in[];

out GS_OUT {
	vec3  normal;
	float sign;
} gs_out;

void main(){
	vec4  center = gl_in[0].gl_Position;
	float radius = gs_in[0].radius;
	float height = gs_in[0].height;

	vec4 points[8] = {
		center + vec4( radius,  height,  radius, 0.0f),  // 0
		center + vec4(-radius,  height,  radius, 0.0f),  // 1
		center + vec4(-radius,  height, -radius, 0.0f),  // 2
		center + vec4( radius,  height, -radius, 0.0f),  // 3

		center + vec4( radius, -height,  radius, 0.0f),  // 4
		center + vec4(-radius, -height,  radius, 0.0f),  // 5
		center + vec4(-radius, -height, -radius, 0.0f),  // 6
		center + vec4( radius, -height, -radius, 0.0f)   // 7
	};

	vec3 normals[8] = {
		normalize(vec3( 1,  1,  1)),  // 0
		normalize(vec3(-1,  1,  1)),  // 1
		normalize(vec3(-1,  1, -1)),  // 2
		normalize(vec3( 1,  1, -1)),  // 3

		normalize(vec3( 1, -1,  1)),  // 4
		normalize(vec3(-1, -1,  1)),  // 5
		normalize(vec3(-1, -1, -1)),  // 6
		normalize(vec3( 1, -1, -1))  // 7
	};

	const int indices[24] = {
		// Front facing - CCW
		0, 3, 1, 2,
		0, 1, 4, 5,
		1, 2, 5, 6,

		// Back Facing - CW
		4, 5, 7, 6,
		3, 7, 2, 6,
		3, 0, 7, 4
	};

	for(int i = 0; i < 6; i++){
		for(int j = 0; j < 4; j++){
			gl_Position   = mvp * points[indices[4*i+j]];	
			gs_out.normal = normals[indices[4*i+j]]; 
			gs_out.sign   = gs_in[0].sign;
			EmitVertex();	
		}
		EndPrimitive();
	}
}