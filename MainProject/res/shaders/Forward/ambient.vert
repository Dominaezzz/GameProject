#version 400

in vec3 position;
in vec2 textureCoords;
in vec3 normal;

in vec4 weights;
in vec4 joints;

varying vec2 v_textureCoordinates;

uniform mat4 transformationMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

uniform mat4 bones[64];

void main(){

	mat4 animationMatrix = (weights[0] * bones[int(joints[0])]) +
	(weights[1] * bones[int(joints[1])]) +
	(weights[2] * bones[int(joints[2])]) +
	(weights[3] * bones[int(joints[3])]);
	
	gl_Position = projectionMatrix * viewMatrix * transformationMatrix * animationMatrix * vec4(position, 1.0);

	v_textureCoordinates = textureCoords;
}
