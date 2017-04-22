#include "tranform.h"

Transform::Transform() : position(0), rotation(), scale(1)
{
}

Matrix4 Transform::getTransform() const
{
	return Matrix4::translate(position) * Matrix4::createFromQuaternion(rotation) * Matrix4::scale(scale);
}
