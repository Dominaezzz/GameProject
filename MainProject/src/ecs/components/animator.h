#pragma once
#include "../component.h"
#include "transform.h"
#include <set>

template<typename T>
struct KeyFrame
{
	const float timeFrame;
	const T value;

	KeyFrame(float time_frame, const T& value) : timeFrame(time_frame), value(value) { }
};

template<typename T>
inline bool operator <(const KeyFrame<T>& left, const KeyFrame<T>& right)
{
	return left.timeFrame < right.timeFrame;
}

template<typename T>
inline bool operator >(const KeyFrame<T>& left, const KeyFrame<T>& right)
{
	return left.timeFrame > right.timeFrame;
}

struct NodeAnimation
{
	Transform* node;
	std::set<KeyFrame<Vector3>> translations;
	std::set<KeyFrame<Quaternion>> rotations;
	std::set<KeyFrame<Vector3>> scales;
};

struct Animation
{
	float duration;
	std::vector<NodeAnimation> nodeAnimations;
};

struct Animator : Component
{
	float speed = 1.0f;
	std::vector<Animation> animations;
};
