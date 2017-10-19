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

	struct Cmp
	{
		using is_transparent = void;

		bool operator()(const KeyFrame& l, const KeyFrame& r) const
		{
			return l.timeFrame < r.timeFrame;
		}

		bool operator()(const KeyFrame& l, const float& r) const
		{
			return l.timeFrame < r;
		}

		bool operator()(const float& l, const KeyFrame& r) const
		{
			return l < r.timeFrame;
		}
	};
};

template<class T> using KeyFrames = std::set<KeyFrame<T>, typename KeyFrame<T>::Cmp>;

struct NodeAnimation
{
	Transform* node;
	KeyFrames<Vector3> translations;
	KeyFrames<Quaternion> rotations;
	KeyFrames<Vector3> scales;
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
