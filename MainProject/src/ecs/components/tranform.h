#pragma once
#include "../component.h"
#include "../../math/vector.h"
#include "../../math/quaternion.h"
#include "../../math/matrix.h"

struct Transform : Component
{
	Vector3 position;
	Quaternion rotation;
	Vector3 scale;

	Transform();
	Matrix4 getTransform() const;
};
