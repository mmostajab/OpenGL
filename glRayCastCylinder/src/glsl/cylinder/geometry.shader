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
	vec3  normalized_pos;
  vec3  center;
  float radius;
  float height;
	float sign;
} gs_out;

void main(){
	vec4  center = gl_in[0].gl_Position;
	float radius = gs_in[0].radius;
	float height = gs_in[0].height;

	vec4 points[8] = {
		center + vec4( radius,  height / 2.0f,  radius, 0.0f),  // 0
		center + vec4(-radius,  height / 2.0f,  radius, 0.0f),  // 1
		center + vec4(-radius,  height / 2.0f, -radius, 0.0f),  // 2
		center + vec4( radius,  height / 2.0f, -radius, 0.0f),  // 3

		center + vec4( radius, -height / 2.0f,  radius, 0.0f),  // 4
		center + vec4(-radius, -height / 2.0f,  radius, 0.0f),  // 5
		center + vec4(-radius, -height / 2.0f, -radius, 0.0f),  // 6
		center + vec4( radius, -height / 2.0f, -radius, 0.0f)   // 7
	};

	vec3 normals[8] = {
		vec3( 1,  1,  1),  // 0
		vec3(-1,  1,  1),  // 1
		vec3(-1,  1, -1),  // 2
		vec3( 1,  1, -1),  // 3

		vec3( 1, -1,  1),  // 4
		vec3(-1, -1,  1),  // 5
		vec3(-1, -1, -1),  // 6
		vec3( 1, -1, -1)   // 7
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
			gl_Position           = mvp * points[indices[4*i+j]];	
			gs_out.normalized_pos = normals[indices[4*i+j]];
			gs_out.sign           = gs_in[0].sign;
      gs_out.center         = center.xyz;
      gs_out.radius         = radius;
      gs_out.height         = height;
			EmitVertex();	
		}
		EndPrimitive();
	}
}