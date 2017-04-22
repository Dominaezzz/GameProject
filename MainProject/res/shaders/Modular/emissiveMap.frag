#version 400

in vec2 v_textureCoordinates;

uniform vec4 emissiveColor;
uniform sampler2D emissiveMap;

vec4 calculateEmissiveColor()
{
	return emissiveColor * texture(emissiveMap, v_textureCoordinates);
}
