#version 420

layout (location = 0) in vec3 position;

out vec3 v_textureCoords;

layout (std140, binding = 0) uniform Matrices
{
	mat4 projectionMatrix;
	mat4 viewMatrix;
	vec3 cameraPosition;
};

void main()
{
	mat4 view = viewMatrix;
	view[3][0] = 0;
	view[3][1] = 0;
	view[3][2] = 0;
	view[3][3] = 1;
	
	gl_Position = projectionMatrix * view * vec4(position, 1.0);
	v_textureCoords = position;
}