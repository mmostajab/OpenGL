#version 450 core

layout ( location = 0 ) uniform mat4 mvp;

layout ( location = 0 ) in vec3  start;
layout ( location = 1 ) in vec3  end;
layout ( location = 2 ) in float width;
layout ( location = 3 ) in float height;
layout ( location = 4 ) in int   sign;

out VS_OUT {

	vec3  start;
	vec3  end;
	float width;
	float height;
	float sign;

} vs_out;

void main() {
	gl_Position   = vec4(start, 1.0f);
	vs_out.start  = start;
	vs_out.end    = end;
	vs_out.width  = width;
	vs_out.height = height;
	vs_out.sign   = float(sign);
} 