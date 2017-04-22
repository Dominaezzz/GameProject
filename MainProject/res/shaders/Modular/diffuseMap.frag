#version 400

in vec2 v_textureCoordinates;

uniform vec4 diffuseColor;
uniform sampler2D diffuseMap;

vec4 calculateDiffuseColor()
{
	return diffuseColor * texture(diffuseMap, v_textureCoordinates);
}
