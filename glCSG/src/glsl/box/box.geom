#version 450 core

layout ( location = 0 ) uniform mat4 mvp;

layout(points) in;
layout(triangle_strip, max_vertices=24) out;

in VS_OUT {

	vec3  dims;
	float sign;

} gs_in[];

out GS_OUT {
	vec3 normal;
	float sign;
} gs_out;

void main(){
	vec4  center = gl_in[0].gl_Position;
	vec4  dims   = vec4(gs_in[0].dims, 0.0f);

	vec4 points[8] = {
		center + vec4( 1.0f,  1.0f,  1.0f, 0.0f) * dims / 2.0f,  // 0
		center + vec4(-1.0f,  1.0f,  1.0f, 0.0f) * dims / 2.0f,  // 1
		center + vec4(-1.0f,  1.0f, -1.0f, 0.0f) * dims / 2.0f,  // 2
		center + vec4( 1.0f,  1.0f, -1.0f, 0.0f) * dims / 2.0f,  // 3

		center + vec4( 1.0f, -1.0f,  1.0f, 0.0f) * dims / 2.0f,  // 4
		center + vec4(-1.0f, -1.0f,  1.0f, 0.0f) * dims / 2.0f,  // 5
		center + vec4(-1.0f, -1.0f, -1.0f, 0.0f) * dims / 2.0f,  // 6
		center + vec4( 1.0f, -1.0f, -1.0f, 0.0f) * dims / 2.0f  // 7
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
		0, 1, 3, 2,
		4, 5, 7, 6,
		0, 3, 4, 7,
		3, 2, 7, 6,
		1, 0, 5, 4,
		2, 1, 6, 5
	};

	for(int i = 0; i < 6; i++){
		for(int j = 0; j < 4; j++){
			gl_Position   = mvp * points[indices[4*i+j]];	
			gs_out.normal = normals[indices[4*i+j]]; 
			EmitVertex();	
		}
		EndPrimitive();
	}
}