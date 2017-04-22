#version 400

const float TILING = 40.0;

in vec2 v_textureCoordinates;

uniform sampler2D backgroundTexture;
uniform sampler2D blendMap;
uniform sampler2D rTexture;
uniform sampler2D gTexture;
uniform sampler2D bTexture;

vec4 calculateDiffuseColor()
{
	vec4 blend = texture(blendMap, v_textureCoordinates);
	vec2 tiledCoords = v_textureCoordinates * TILING;
	
	vec4 backgroundColor = texture(backgroundTexture, tiledCoords) * (1 - (blend.r + blend.g + blend.b));
	vec4 rColor = texture(rTexture, tiledCoords) * blend.r;
	vec4 gColor = texture(gTexture, tiledCoords) * blend.g;
	vec4 bColor = texture(bTexture, tiledCoords) * blend.b;
	
	return backgroundColor + rColor + gColor + bColor;
}
