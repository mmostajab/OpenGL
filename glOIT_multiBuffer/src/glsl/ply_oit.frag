#version 430 core

//layout (early_fragment_tests) in;

layout (binding = 0, r32ui) uniform uimage2D head_pointer_image;
layout (binding = 1, rgba32ui) uniform writeonly uimageBuffer list_buffer;
layout (binding = 0, offset = 0) uniform atomic_uint list_counter;

layout (location = 0) uniform float alpha_val ;
layout (location = 2) uniform int ground;
layout (location = 3) uniform float radius;

layout (location = 0) out vec4 color;
layout (location = 1) out vec4 normal_depth;

// Head pointer 2D buffer

layout(std140, binding = 3) uniform GeneralBlock {
    float color_multiplier;
	vec4 color_background;
} ;

in VS_OUT {
    vec3 view_dir_view;
    vec3 light_dir_view;
    vec3 normal_view;
    vec4 color;
	vec4 world_pos;
} fs_in;

uniform float specular_power = 128.0f; 

uniform vec3 diffuse_albedo  = vec3(1.0f, 1.0f, 1.0f);
uniform vec3 specular_albedo = vec3(0.7f);

uniform float shading_level = 1.0;

void main(void)
{    
	uint index;
    uint old_head;
    uvec4 item;
    vec4 frag_color;
    vec4 frag_specularity;

		vec3 v = normalize(fs_in.view_dir_view);
		vec3 l = normalize(fs_in.light_dir_view);
		vec3 n = normalize(fs_in.normal_view);
		vec3 r = reflect(-l, n);
		vec3 h = normalize(l + v);
    
		vec3 ambient_factor  = vec3(0.20f, 0.20f, 0.20f);
		vec3 diffuse_factor  = diffuse_albedo * max(dot(n,l), 0.0f);
		vec3 specular_factor = specular_albedo * pow(max(dot(r, v), 0.0f), specular_power); 

		frag_color	 = 40.0f * fs_in.color * mix(vec4(0.0), vec4( ambient_factor + diffuse_factor, alpha_val), shading_level);
		frag_color.w = alpha_val;
		frag_specularity = fs_in.color * vec4(specular_factor, 0.0f);

		if(length(fs_in.normal_view) == 0){
			frag_color = fs_in.color;
			frag_specularity = vec4(0.0f, 0.0f, 0.0f, 0.0f);
		}


    //frag_color = fs_in.color;
	
	  index = atomicCounterIncrement(list_counter);
	  old_head = imageAtomicExchange(head_pointer_image, ivec2(gl_FragCoord.xy), uint(index));

    item.x = old_head;
    item.y = packUnorm4x8(frag_color);
    item.z = floatBitsToUint(gl_FragCoord.z);
    item.w = packUnorm4x8(frag_specularity);

    imageStore(list_buffer, int(index), item);

    color = frag_color;
    normal_depth = vec4(0.0f, 0.0f, 0.0f, 0.0f);
}