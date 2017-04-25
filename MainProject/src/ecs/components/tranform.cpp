#include "tranform.h"

void Transform::invalidate() const
{
	worldTransform.reset();
	worldPosition.reset();
	worldRotation.reset();
	worldScale.reset();
}

Transform::Transform()
{
}

void Transform::setPosition(const Vector3& position)
{
	localTransform.reset();
	invalidate();
	this->position = position;
}

void Transform::setRotation(const Quaternion& rotation)
{
	localTransform.reset();
	invalidate();
	this->rotation = rotation;
}

void Transform::setScale(const Vector3& scale)
{
	localTransform.reset();
	invalidate();
	this->scale = scale;
}

const Vector3& Transform::getPosition() const
{
	return position;
}

const Quaternion& Transform::getRotation() const
{
	return rotation;
}

const Vector3& Transform::getScale() const
{
	return scale;
}

const Vector3& Transform::getWorldPosition() const
{
	if (worldPosition == nullptr)
	{
		worldPosition = std::make_unique<Vector3>(getTransform().extractTranslation());
	}
	return *worldPosition;
}

const Quaternion& Transform::getWorldRotation() const
{
	if(worldRotation == nullptr)
	{
		worldRotation = std::make_unique<Quaternion>(getTransform().extractRotation());
	}
	return *worldRotation;
}

const Vector3& Transform::getWorldScale() const
{
	if(worldScale == nullptr)
	{
		worldScale = std::make_unique<Vector3>(getTransform().extractScale());
	}
	return *worldScale;
}

const Matrix4& Transform::getLocalTransform() const
{
	if(localTransform == nullptr)
	{
		localTransform = std::make_unique<Matrix4>(Matrix4::translate(position) * Matrix4::createFromQuaternion(rotation) * Matrix4::scale(scale));
	}
	return *localTransform;
}

const Matrix4& Transform::getTransform() const
{
	if(worldTransform == nullptr)
	{
		auto p = parent.lock();
		if(p != nullptr)
		{
			worldTransform = std::make_unique<Matrix4>(p->getTransform() * getLocalTransform());
		}
		else
		{
			worldTransform = std::make_unique<Matrix4>(getLocalTransform());
		}
	}
	return *worldTransform;
}
