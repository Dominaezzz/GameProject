#include "transform.h"

void Transform::invalidateHeirachy() const
{
	if (worldTransform != nullptr)
	{
		worldTransform.reset();
		worldPosition.reset();
		worldRotation.reset();
		worldScale.reset();

		for (const Transform* child : children)
		{
			child->invalidateHeirachy();
		}
	}
}

Transform::Transform()
{
}

void Transform::setPosition(const Vector3& position)
{
	localTransform.reset();
	invalidateHeirachy();
	this->position = position;
}

void Transform::setRotation(const Quaternion& rotation)
{
	localTransform.reset();
	invalidateHeirachy();
	this->rotation = rotation;
}

void Transform::setScale(const Vector3& scale)
{
	localTransform.reset();
	invalidateHeirachy();
	this->scale = scale;
}

void Transform::setParent(Transform * transform)
{
	transform->addChild(this);
}

void Transform::addChild(Transform * transform)
{
	children.push_back(transform);
	transform->parent = this;
	transform->invalidateHeirachy();
}

Vector3& Transform::getPosition()
{
	localTransform.reset();
	invalidateHeirachy();
	return position;
}

Quaternion& Transform::getRotation()
{
	localTransform.reset();
	invalidateHeirachy();
	return rotation;
}

Vector3& Transform::getScale()
{
	localTransform.reset();
	invalidateHeirachy();
	return scale;
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
		if(parent != nullptr)
		{
			worldTransform = std::make_unique<Matrix4>(parent->getTransform() * getLocalTransform());
		}
		else
		{
			worldTransform = std::make_unique<Matrix4>(getLocalTransform());
		}
	}
	return *worldTransform;
}
