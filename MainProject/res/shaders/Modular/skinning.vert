#version 400

const int MAX_BONE_WEIGHTS = 10;

layout(location = 6) in vec2 boneWeights[MAX_BONE_WEIGHTS];

uniform mat4 bones[70];

mat4 calculateAnimation()
{
	mat4 animationMatrix = mat4(0.0);
	
	for(int i = 0; i < MAX_BONE_WEIGHTS; i++)
	{
		vec2 boneWeight = boneWeights[i];
		if(boneWeight.y == 0)break;
		animationMatrix += boneWeight.y * bones[int(boneWeight.x)];
	}
	
	return animationMatrix;
}
