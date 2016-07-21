#version 330 compatibility

#extension GL_ARB_separate_shader_objects: enable
#extension GL_ARB_uniform_buffer_object:   enable

layout(std140, binding = 0) uniform TransformBlock {
    mat4 proj_mat;			// projection matrix
    mat4 view_mat;			// view matrix
    mat4 world_mat;			// world matrix
	mat4 normal_mat;		// transpose of inverse modelview matrix 

	mat4 modelView_mat;		// modelview matrix
	mat4 mvp_mat;
} ;

struct Vertex {
	vec3 position;
	vec3 normal;
};

layout ( location = 0 ) Vertex vertex;

out VS_OUT {
	vec3 positionViewSpace;			// view space position
	vec3 normalViewSpace;			// view space normal
} vs_out;

void main()
{
	//
	// multiply the object space vertex position with the modelview matrix 
	// to get the eye space vertex position
	//
	vs_out.positionViewSpace = (modelView_mat * vertex.position).xyz;

	//
	// multiply the object space normal with the normal matrix (transpose of the inverse 
	// model view matrix) to get the eye space normal
	//
	vs_out.normalViewSpace   = normal_mat * vertex.normal;

	//
	// multiply the combiend modelview projection matrix with the object space vertex
	// position to get the clip space position
	//
	gl_Position = mvp * vertex. position;
}