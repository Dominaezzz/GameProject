#version 400

layout(location = 6) in vec4 joints;
layout(location = 7) in vec4 weights;

uniform mat4 bones[70];

mat4 calculateAnimation()
{
	mat4 animationMatrix = mat4(0.0);

	for(int i = 0; i < 4; i++)
	{
		if(weights[i] == 0)break;
		animationMatrix += weights[i] * bones[int(joints[i])];
	}
	
	return animationMatrix;
}
