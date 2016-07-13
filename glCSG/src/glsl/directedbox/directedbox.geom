#version 450 core

layout ( location = 0 ) uniform mat4 mvp;

layout(points) in;
layout(triangle_strip, max_vertices=24) out;

in VS_OUT {

	vec3  start;
	vec3  end;
	float width;
	float height;
	float sign;

} gs_in[];

out GS_OUT {
	vec3 normal;
	float sign;
} gs_out;

void main(){
	vec3  start  = gs_in[0].start;
	vec3  end    = gs_in[0].end;
	float width  = gs_in[0].width;
	float height = gs_in[0].height;
	float sign   = gs_in[0].sign;

	vec3 x = normalize(end - start);
	vec3 y = vec3(0.0f, 1.0f, 0.0f);
	vec3 z = normalize(cross(x, y));
	y      = normalize(cross(z, x));


	vec3 points[8] = {

		start + z * width / 2.0f + y * height / 2.0f, // 0
		start - z * width / 2.0f + y * height / 2.0f, // 1
		start - z * width / 2.0f - y * height / 2.0f, // 2
		start + z * width / 2.0f - y * height / 2.0f, // 3

		end + z * width / 2.0f + y * height / 2.0f,   // 4
		end - z * width / 2.0f + y * height / 2.0f,   // 5
		end - z * width / 2.0f - y * height / 2.0f,   // 6
		end + z * width / 2.0f - y * height / 2.0f,   // 7
	};

	vec3 normals[8] = {
		normalize(+x+y+z),  // 0
		normalize(-x+y+z),  // 1
		normalize(-x+y-z),  // 2
		normalize(+x+y-z),  // 3

		normalize(+x-y+z),  // 4
		normalize(-x-y+z),  // 5
		normalize(-x-y-z),  // 6
		normalize(+x-y-z)   // 7
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
			gl_Position   = mvp * vec4(points[indices[4*i+j]], 1.0f);	
			gs_out.normal = normals[indices[4*i+j]]; 
			EmitVertex();	
		}
		EndPrimitive();
	}
}