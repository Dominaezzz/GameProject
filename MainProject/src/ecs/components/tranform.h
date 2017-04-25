#pragma once
#include <memory>
#include <vector>
#include "../component.h"
#include "../../math/vector.h"
#include "../../math/quaternion.h"
#include "../../math/matrix.h"

struct Transform : Component
{
private:
	Vector3 position;
	Quaternion rotation;
	Vector3 scale = Vector3(1);
	std::weak_ptr<Transform> parent;
	std::vector<std::weak_ptr<Transform>> children;

	mutable std::unique_ptr<Vector3> worldPosition = nullptr;
	mutable std::unique_ptr<Quaternion> worldRotation = nullptr;
	mutable std::unique_ptr<Vector3> worldScale = nullptr;
	mutable std::unique_ptr<Matrix4> worldTransform = nullptr;
	mutable std::unique_ptr<Matrix4> localTransform = nullptr;

	void invalidate() const;
public:
	Transform();

	void setPosition(const Vector3& position);
	void setRotation(const Quaternion& rotation);
	void setScale(const Vector3& scale);

	const Vector3& getPosition() const;
	const Quaternion& getRotation() const;
	const Vector3& getScale() const;
	const Vector3& getWorldPosition() const;
	const Quaternion& getWorldRotation() const;
	const Vector3& getWorldScale() const;
	const Matrix4& getLocalTransform() const;
	const Matrix4& getTransform() const;

	__declspec(property(get = getPosition, put = setPosition)) Vector3 Position;
	__declspec(property(get = getRotation, put = setRotation)) Quaternion Rotation;
	__declspec(property(get = getScale, put = setScale)) Vector3 Scale;
};
