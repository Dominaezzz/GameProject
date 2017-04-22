#version 400

in vec2 position;

out vec2 texCoords;

void main()
{
    texCoords = position + 0.5;
    gl_Position = vec4(position, 0.0, 1.0);
}
