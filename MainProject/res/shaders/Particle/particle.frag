#version 400

in vec2 v_textureCoordinates;
in vec2 v_textureCoordinates2;
in float v_blend;

out vec4 FragColor;

uniform sampler2D particleTexture;

void main()
{
	vec4 color1 = texture(particleTexture, v_textureCoordinates);
	vec4 color2 = texture(particleTexture, v_textureCoordinates2);
	
	FragColor = mix(color1, color2, v_blend);
}
