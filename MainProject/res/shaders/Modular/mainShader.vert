#version 420

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 textureCoords;

out vec3 v_position;
out vec2 v_textureCoordinates;

layout (std140, binding = 0) uniform Matrices
{
	mat4 projectionMatrix;
	mat4 viewMatrix;
	vec3 cameraPosition;
};
uniform mat4 transformationMatrix;

mat4 calculateAnimation();
void calculateNormal(mat4 transform);

void main(){
	mat4 transform = transformationMatrix * calculateAnimation();
	
	v_textureCoordinates = textureCoords;
	v_position = (transform * vec4(position, 1.0)).xyz;
	calculateNormal(transform);
	
	gl_Position = projectionMatrix * viewMatrix * vec4(v_position, 1.0);
}
