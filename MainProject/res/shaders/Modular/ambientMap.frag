#version 400

in vec2 v_textureCoordinates;

uniform vec4 ambientColor;
uniform sampler2D ambientMap;

vec4 calculateAmbientColor()
{
	return ambientColor * texture(ambientMap, v_textureCoordinates);
}
