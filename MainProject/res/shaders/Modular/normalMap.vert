#version 400

layout(location = 2) in vec3 normal;
layout(location = 3) in vec3 tangent;
layout(location = 4) in vec3 bitangent;

out mat3 v_tbn; // tangent space to world space.

void calculateNormal(mat4 transform)
{
	vec3 N = normalize((transform * vec4(normal, 0.0)).xyz);
	vec3 T = normalize((transform * vec4(tangent, 0.0)).xyz);
	vec3 B = normalize((transform * vec4(bitangent, 0.0)).xyz);
	
	T = normalize(T - dot(T, N) * N);
	B = normalize(B - dot(B, N) * N);
	
	v_tbn = mat3(T, B, N);
}
