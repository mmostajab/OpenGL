#version 430 core

layout(lines) in;
layout(line_strip, max_vertices=100) out;

in VS_OUT{
	vec4 color;
	vec4 control_point0;
	vec4 control_point1;
} gs_in[];

out GS_OUT{
  vec4 color;
} gs_out;

uniform int curve_detail = 20;

// Found in nvidia sdk
vec4 evaluateBezierPosition( vec4 v[4], float t )
{
    float OneMinusT = 1.0 - t;
    float b0 = OneMinusT*OneMinusT*OneMinusT;
    float b1 = 3.0*t*OneMinusT*OneMinusT;
    float b2 = 3.0*t*t*OneMinusT;
    float b3 = t*t*t;
    return b0*v[0] + b1*v[1] + b2*v[2] + b3*v[3];
}


//mat4 u_mat = mat4( vec4( -1, 3, -3, 1), vec4(3, -6, 3, 0), vec4(-3, 0, 3, 0), vec4(1, 4, 1, 0) );
mat4 norm_mat  = mat4( vec4( -1, 3, -3, 1), vec4(3, -6, 3, 0), vec4(-3, 0, 3, 0), vec4(1, 4, 1, 0) );
mat4 first_mat = mat4( vec4( -1.0f, 7.0f/6.0f, -1.0f/3.0f, 1.0f/6.0f), vec4(+3.0f, -3.0f, 0, 0), vec4(-3.0f, 2.0f, 1.0f, 0.0f), vec4(1.0f, 0.0f, 0.0f, 0.0f) );
mat4 last_mat  = mat4( vec4( -1.0f/6.0f, 1.0f/3.0f, -7.0f/6.0f, 1.0f), vec4(1.0f/2.0f, -1.0f, +1.0f/2.0f, 0), vec4(-1.0f/2.0f, 0, +1.0f/2.0f, 0.0f), vec4(1.0f/6.0f, 2.0f/3.0f, 1.0f/6.0f, 0.0f) );

vec4 evaluateBSplinePosition( vec4 v[4], float t )
{
	// first curve
	if(gl_PrimitiveIDIn == 0) return 1 / 6.0f * mat4(v[0], v[1], v[2], v[3]) * first_mat *  vec4(t*t*t, t*t, t, 1);
	if(gl_PrimitiveIDIn == 15) return 1 / 6.0f * mat4(v[0], v[1], v[2], v[3]) * last_mat *  vec4(t*t*t, t*t, t, 1);
	return 1 / 6.0f * mat4(v[0], v[1], v[2], v[3]) * norm_mat *  vec4(t*t*t, t*t, t, 1);
}

void main()
{

//#define SHOW_LINES
#ifdef SHOW_LINES
	gl_Position = gl_in[0].gl_Position;
	gs_out.color = vec4(1.0f);
	EmitVertex();

	gl_Position = gl_in[1].gl_Position;
	gs_out.color = vec4(1.0f);
	EmitVertex();

	EndPrimitive();
	return;
#endif


    vec4 pos[4];
    pos[0] = gl_in[0].gl_Position;
    pos[1] = gs_in[0].control_point0;
    pos[2] = gs_in[0].control_point1;
    pos[3] = gl_in[1].gl_Position;
    float OneOverDetail = 1.0f / float(curve_detail-1.0f);
    for( int i=0; i<curve_detail; i++ )
    {
        float t = i * OneOverDetail;
        //vec4 p = evaluateBezierPosition( pos, t );
		vec4 p = evaluateBSplinePosition( pos, t );
        gl_Position = p;
		gs_out.color = vec4(1.0f);
        EmitVertex();
    }
 
    EndPrimitive();
}