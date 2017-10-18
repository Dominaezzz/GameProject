#include "animation_system.h"

AnimationSystem::AnimationSystem(World * world) : System(world)
{
}

template<typename T>
static T findCurrent(const std::set<KeyFrame<T>>& keyFrames, float elapsedTime, const T& def, const std::function<T (const T&, const T&, float)>& interpolation)
{
	for (auto it = keyFrames.begin(); it != keyFrames.end(); ++it)
	{
		const KeyFrame<T>& keyFrame = *it;
		if (keyFrame.timeFrame > elapsedTime)
		{
			const KeyFrame<T>& previousKeyFrame = it == keyFrames.begin() ? *keyFrames.rbegin() : *--it;
			const KeyFrame<T>& nextKeyFrame = keyFrame;
			float blend = (elapsedTime - previousKeyFrame.timeFrame) / (nextKeyFrame.timeFrame - previousKeyFrame.timeFrame);

			return interpolation(previousKeyFrame.value, nextKeyFrame.value, blend);
		}
	}
	return def;
}

void AnimationSystem::update(float dt)
{
	for (AnimationNode& node : nodes)
	{
		Animator* animator = node.get<Animator>();

		const Animation& animation = animator->animations[0];

		node.elapsedTime += dt * animator->speed;
		while (node.elapsedTime > animation.duration) node.elapsedTime -= animation.duration;

		for(const NodeAnimation& nodeAnimation : animation.nodeAnimations)
		{
			Transform* transorm = nodeAnimation.node;
			transorm->Position = findCurrent<Vector3>(nodeAnimation.translations, node.elapsedTime, Vector3(), vec::lerp<3, float>);
			transorm->Rotation = findCurrent<Quaternion>(nodeAnimation.rotations, node.elapsedTime, Quaternion(), Quaternion::slerp);
			transorm->Scale = findCurrent<Vector3>(nodeAnimation.scales, node.elapsedTime, Vector3(1), vec::lerp<3, float>);
		}
	}
}

void AnimationSystem::onGameObjectEdited(GameObject* gameObject)
{
	nodes.reportChange(gameObject);
}
