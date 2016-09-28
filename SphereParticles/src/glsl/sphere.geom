#version 120
#extension GL_EXT_geometry_shader4 : enable
   
uniform float radius = 0.7;

varying out vec4 viewPos;
varying out vec4 viewCenter;

void main()
{
    vec4 center = gl_ModelViewMatrix * gl_PositionIn[0];
	vec4 pos = center;

    vec4 viewX = vec4(1, 0, 0, 0);
    vec4 viewY = vec4(0, 1, 0, 0);

	viewCenter = gl_ModelViewMatrix * gl_PositionIn[0];
	viewPos    = viewCenter + radius * ( -viewX -viewY);
    gl_Position = gl_ProjectionMatrix * viewPos;
    EmitVertex();

    viewCenter = gl_ModelViewMatrix * gl_PositionIn[0];
	viewPos    = viewCenter + radius * ( +viewX -viewY);
    gl_Position = gl_ProjectionMatrix * viewPos;
    EmitVertex();

    viewCenter = gl_ModelViewMatrix * gl_PositionIn[0];
	viewPos    = viewCenter + radius * ( -viewX +viewY);
    gl_Position = gl_ProjectionMatrix * viewPos;
    EmitVertex();

    viewCenter = gl_ModelViewMatrix * gl_PositionIn[0];
	viewPos    = viewCenter + radius * ( +viewX +viewY);
    gl_Position = gl_ProjectionMatrix * viewPos;
    EmitVertex();

    EndPrimitive();

}