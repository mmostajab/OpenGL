#version 430 core

out vec2 tex_coord;

layout(location = 1) uniform mat4 transform;

void main(void)
{
    const vec4 vertices[] = vec4[]( vec4(-1.0, -1.0, 0.5, 1.0),
                                    vec4( 1.0, -1.0, 0.5, 1.0),
                                    vec4(-1.0,  1.0, 0.5, 1.0),
                                    vec4( 1.0,  1.0, 0.5, 1.0) );

	const vec2 tex_coords[] =  vec2[]( 
									vec2( 0.0, 0.0),
                                    vec2( 1.0, 0.0),
                                    vec2( 0.0, 1.0),
                                    vec2( 1.0, 1.0) 
								);

    gl_Position        = transform * vertices[gl_VertexID];
	tex_coord          = tex_coords[gl_VertexID];
}
