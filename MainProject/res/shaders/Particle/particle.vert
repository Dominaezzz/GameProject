#version 420

layout(location = 0) in vec2 position;

out vec2 v_textureCoordinates;
out vec2 v_textureCoordinates2;
out float v_blend;

layout (std140, binding = 0) uniform Matrices
{
	mat4 projectionMatrix;
	mat4 viewMatrix;
};
uniform vec4 transform;

uniform int index;
uniform float blend;
uniform int numberOfRows;

mat4 translate(float x, float y, float z);
mat4 scale(float x, float y, float z);

void main()
{
	v_blend = blend;
	
	int row = index % numberOfRows;
	int column = (index - row) / numberOfRows;
	v_textureCoordinates = (position + vec2(row, column)) / numberOfRows;
	
	int row2 = (index + 1) % numberOfRows;
	int column2 = (index + 1 - row2) / numberOfRows;
	v_textureCoordinates2 = (position + vec2(row2, column2)) / numberOfRows;
	
	mat4 modelView = viewMatrix * translate(transform.x, transform.y, transform.z);
	modelView[0].xyz = vec3(transform.w, 0, 0);
	modelView[1].xyz = vec3(0, transform.w, 0);
	modelView[2].xyz = vec3(0, 0, transform.w);
	
	gl_Position = projectionMatrix * modelView * vec4(position, 0.0, 1.0);
}
