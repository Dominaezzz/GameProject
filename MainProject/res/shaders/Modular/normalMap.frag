#version 400

in vec2 v_textureCoordinates;
in mat3 v_tbn; // tangent space to world space.

uniform sampler2D normalMap;

vec3 calculateNormal()
{
	vec3 normal = texture(normalMap, v_textureCoordinates).xyz * (256.0 / 255.0);
	normal = (2.0 * normal) - 1.0;
	return normalize(v_tbn * normal);
}
