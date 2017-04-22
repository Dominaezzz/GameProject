#version 400

layout(location = 2) in vec3 normal;

out vec3 v_normal;

void calculateNormal(mat4 transform)
{
	v_normal = (transform * vec4(normal, 0.0)).xyz;
}
