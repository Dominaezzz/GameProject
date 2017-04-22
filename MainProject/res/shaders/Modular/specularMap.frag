#version 400

in vec2 v_textureCoordinates;

uniform vec4 specularColor;
uniform sampler2D specularMap;

vec4 calculateSpecularColor()
{
	return specularColor * texture(specularMap, v_textureCoordinates);
}
