#version 450 core

layout(points) in;
layout(triangle_strip, max_vertices=24) out;

in VS_OUT {

	float radius;
	float height;

} gs_in[];

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

	gl_Position = points[0];	EmitVertex();
	gl_Position = points[1];	EmitVertex();
	gl_Position = points[2];	EmitVertex();
	gl_Position = points[3];	EmitVertex();
	EndPrimitive();

	gl_Position = points[4];	EmitVertex();
	gl_Position = points[5];	EmitVertex();
	gl_Position = points[6];	EmitVertex();
	gl_Position = points[7];	EmitVertex();
	EndPrimitive();

	gl_Position = points[4];	EmitVertex();
	gl_Position = points[5];	EmitVertex();
	gl_Position = points[6];	EmitVertex();
	gl_Position = points[7];	EmitVertex();
	EndPrimitive();

	gl_Position = points[4];	EmitVertex();
	gl_Position = points[5];	EmitVertex();
	gl_Position = points[6];	EmitVertex();
	gl_Position = points[7];	EmitVertex();
	EndPrimitive();

	gl_Position = points[4];	EmitVertex();
	gl_Position = points[5];	EmitVertex();
	gl_Position = points[6];	EmitVertex();
	gl_Position = points[7];	EmitVertex();
	EndPrimitive();

	gl_Position = points[4];	EmitVertex();
	gl_Position = points[5];	EmitVertex();
	gl_Position = points[6];	EmitVertex();
	gl_Position = points[7];	EmitVertex();
	EndPrimitive();
}