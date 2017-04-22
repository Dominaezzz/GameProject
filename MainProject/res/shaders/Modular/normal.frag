#version 400

in vec3 v_normal;

vec3 calculateNormal()
{
	return normalize(v_normal);
}
