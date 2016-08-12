#version 330 compatibility

#extension GL_ARB_separate_shader_objects: enable
#extension GL_ARB_uniform_buffer_object:   enable

const int num_lights    =   1;
const int num_materials = 100;

struct Light
{
	ivec4 type;             // (dir,point,spot) value is either 1 or 0 and maximal one component can be 1
	vec4 position;          // in eye space
	vec4 spot_direction;    // in eye space

	vec4 Ia;                // ambient  max. Intensity
	vec4 Id;                // diffuse  max. Intensity
	vec4 Is;                // specular max. Intensity

	vec4 attenuation;       // (constant, linear, quadratic) with constant >= 1 and linear,quadratic >= 0
	
	float spot_cos_cutoff;  // cosine cut of angle

	float spot_exponent;    // [0-128]
};

//layout (std140, binding = 1) uniform Lights
layout (std140) uniform Lights
{
	Light light[num_lights];
} lights;

struct Material
{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec4 emissive;

	float opacity;
	float shininess;
};

//layout (std140, binding = 2) uniform Materials
layout (std140) uniform Materials
{
	Material material[num_materials];
} materials;


//layout (std140, binding = 3) uniform GeomState
layout (std140) uniform GeomState
{
	int material_index;
} geom_state;

const vec3 cCameraPositionES = vec3(0,0,0); // eye is at vec3(0,0,0) in eye space!

in VS_OUT {
	vec3 positionViewSpace;			// view space position
	vec3 normalViewSpace;			// view space normal
} fs_in;

layout ( location = 0 ) out vec4 outColor;

//
// directional light contribution
//
vec3 directionalLight(
	in int i,   // light identifier, i.e. current light
	in int j,   // material identifier
	in vec3 n,  // vertex normal in eye space
	in vec3 v)  // view direction in eye space
{
	if (lights.light[i].type[0] == 0)
		return vec3(0.0, 0.0, 0.0);

	//
	// the light direction in eye space
	//
	vec3 l = -lights.light[i].spot_direction.xyz;    // we carry the directional light direction in the spot_direction slot

	//
	// the half vector
	//
	vec3 h = normalize(l + v);

	float n_dot_l = max(0.0, dot(n, l));
	float n_dot_h = max(0.0, dot(n, h));

	float m = materials.material[j].shininess;

	float pf;           // power factor

	if (n_dot_l == 0.0)
	   pf = 0.0;
	else
	   pf = pow(n_dot_h, m);

	return materials.material[j].emissive.rgb 
		 + lights.light[i].Ia.rgb * materials.material[j].ambient.rgb 
		 + lights.light[i].Id.rgb * materials.material[j].diffuse.rgb  * n_dot_l                   // / PI
		 + lights.light[i].Is.rgb * materials.material[j].specular.rgb * (m+8)*0.0125 * pf;        // * (m+8)/(8*PI)
}

//
// point light contribution
//
vec3 pointLight(
	in int i,   // light identifier, i.e. current light
	in int j,   // material identifier
	in vec3 n,  // vertex normal in eye space
	in vec3 v,  // view direction in eye space
	in vec3 p)  // vertex position in eye space
{
	if (lights.light[i].type[1] == 0)
		return vec3(0.0, 0.0, 0.0);

	vec3  l = vec3(lights.light[i].position.xyz) - p;    // direction from surface to light position
	float d = length(l);                             // distance from surface to light source
	l = normalize(l);                                // normalized direction from surface to light position

	//
	// the half vector
	//
	vec3 h = normalize(l + v);

	float n_dot_l = max(0.0, dot(n, l));
	float n_dot_h = max(0.0, dot(n, h));

	float m = materials.material[j].shininess;

	float pf;           // power factor

	if (n_dot_l == 0.0)
	   pf = 0.0;
	else
	   pf = pow(n_dot_h, m);

	//
	// Compute attenuation
	//
	float attenuation = 1.0 / (lights.light[i].attenuation.x + 
							  (lights.light[i].attenuation.y * d) + 
							  (lights.light[i].attenuation.z * d * d));

	attenuation = clamp(attenuation, 0.0, 1.0);

	return materials.material[j].emissive.rgb 
		 + attenuation * lights.light[i].Ia.rgb * materials.material[j].ambient.rgb 
		 + attenuation * lights.light[i].Id.rgb * materials.material[j].diffuse.rgb  * n_dot_l                   // / PI
		 + attenuation * lights.light[i].Is.rgb * materials.material[j].specular.rgb * (m+8)*0.0125 * pf;        // * (m+8)/(8*PI)
}

//
// spot light contribution
//
vec3 spotLight(
	in int i,   // light identifier, i.e. current light
	in int j,   // material identifier
	in vec3 n,  // vertex normal in eye space
	in vec3 v,  // view direction in eye space
	in vec3 p)  // vertex position in eye space
{
	if (lights.light[i].type[2] == 0)
		return vec3(0.0, 0.0, 0.0);

	vec3  l = vec3(lights.light[i].position.xyz) - p;   // direction from surface to light position
	float d = length(l);                            // distance from surface to light source
	l = normalize(l);                               // normalized direction from surface to light position
	
	vec3 s = lights.light[i].spot_direction.xyz;        // spot direction

	//
	// the half vector
	//
	vec3 h = normalize(l + v);

	float n_dot_l = max(0.0, dot(n, l));
	float n_dot_h = max(0.0, dot(n, h));
	float l_dot_s = dot(-l, s);

	float m = materials.material[j].shininess;

	float pf;           // power factor

	if (n_dot_l == 0.0)
	   pf = 0.0;
	else
	   pf = pow(n_dot_h, m);

	//
	// Compute attenuation
	//
	float attenuation = 1.0 / (lights.light[i].attenuation.x + 
							  (lights.light[i].attenuation.y * d) + 
							  (lights.light[i].attenuation.z * d * d));

	attenuation = clamp(attenuation, 0.0, 1.0);

	if (l_dot_s < lights.light[i].spot_cos_cutoff) 
		attenuation = 0.0;
	else
		attenuation *= pow(l_dot_s, lights.light[i].spot_exponent);

	return materials.material[j].emissive.rgb 
		 + attenuation * lights.light[i].Ia.rgb * materials.material[j].ambient.rgb 
		 + attenuation * lights.light[i].Id.rgb * materials.material[j].diffuse.rgb  * n_dot_l                   // / PI
		 + attenuation * lights.light[i].Is.rgb * materials.material[j].specular.rgb * (m+8)*0.0125 * pf;        // * (m+8)/(8*PI)
}

void main()
{
	//
	// normalize the eye space normal
	//
	vec3 N = normalize(fs_in.normalViewSpace);

	//
	// get the view vector and normalize it
	//
	vec3 V = normalize(cCameraPositionES - fs_in.normalViewSpace);

	//
	// Integrate over all lights: Any unused light does not contribute and each light
	// contribute either from the directional light, the point light or the spot light.
	//
	vec3 color = vec3(0.0, 0.0, 0.0);
	for (int i = 0; i < num_lights; ++i) {
		color += directionalLight(i, geom_state.material_index, N, V) 
			  +        pointLight(i, geom_state.material_index, N, V, fs_in.positionViewSpace) 
			  +         spotLight(i, geom_state.material_index, N, V, fs_in.positionViewSpace);
	}
	outColor = vec4(color, materials.material[geom_state.material_index  ].opacity);
}