#version 120
   
uniform float radius = 0.7;

varying in vec4 viewPos;
varying in vec4 viewCenter;

void main()
{
	vec3 distanceCenter = viewPos.xyz - viewCenter.xyz;

	// early check
    float l = length(distanceCenter);
	if(l >= radius) discard;

	vec3 dir = normalize(viewPos.xyz);
	
	float a = 1.0f;
	float bPrime = -(viewCenter.x * dir.x + viewCenter.y * dir.y + viewCenter.z * dir.z);
	float c = (viewCenter.x * viewCenter.x + viewCenter.y * viewCenter.y + viewCenter.z * viewCenter.z - radius * radius);

	float deltaPrime  = bPrime * bPrime - a * c;

	if(deltaPrime >= 0){
		float t = min(-bPrime + sqrt(deltaPrime), -bPrime - sqrt(deltaPrime));
		vec3 intersect_point = t * dir;
		vec3 viewNormal = normalize(intersect_point - viewCenter.xyz);
		gl_FragColor = (vec4(viewNormal, 1.0) + vec4(1.0)) / 2.0;

		vec4 projected_intersection_point = gl_ProjectionMatrix * vec4(intersect_point, 1.0);
		gl_FragDepth = projected_intersection_point.z  / projected_intersection_point.w ;
		//gl_FragDepth = (projected_intersection_point.z + 1.0f) / 2.0f;
	} 
	else discard;	
}