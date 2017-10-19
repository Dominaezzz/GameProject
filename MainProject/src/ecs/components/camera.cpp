#include "camera.h"

float& Camera::getFieldOfView()
{
	projection.reset();
	return fieldOfView;
}

float& Camera::getNear()
{
	projection.reset();
	return near;
}

float& Camera::getFar()
{
	projection.reset();
	return far;
}

float& Camera::getViewportWidth()
{
	projection.reset();
	return viewportWidth;
}

float& Camera::getViewportHeight()
{
	projection.reset();
	return viewportHeight;
}

const float& Camera::getFieldOfView() const
{
	return fieldOfView;
}

const float& Camera::getNear() const
{
	return near;
}

const float& Camera::getFar() const
{
	return far;
}

const float& Camera::getViewportWidth() const
{
	return viewportWidth;
}

const float& Camera::getViewportHeight() const
{
	return viewportHeight;
}

void Camera::setFieldOfView(float field_of_view)
{
	fieldOfView = field_of_view;
	projection.reset();
}

void Camera::setNear(float near)
{
	this->near = near;
	projection.reset();
}

void Camera::setFar(float far)
{
	this->far = far;
	projection.reset();
}

void Camera::setViewportWidth(float viewport_width)
{
	viewportWidth = viewport_width;
	projection.reset();
}

void Camera::setViewportHeight(float viewport_height)
{
	viewportHeight = viewport_height;
	projection.reset();
}

const Matrix4& Camera::getProjection() const
{
	if (projection == nullptr)
	{
		projection = std::make_unique<Matrix4>(Matrix4::perspective(fieldOfView, viewportWidth / viewportHeight, near, far));
	}
	return *projection;
}
