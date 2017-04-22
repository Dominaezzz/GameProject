#version 400

in vec2 texCoords;

uniform sampler2D diffuse;

out vec4 FragColor;

void main()
{
    FragColor = texture(diffuse, texCoords);
}
