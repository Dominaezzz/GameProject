#include "camera.h"

Matrix4 Camera::getProjection()
{
	return Matrix4::perspective(fieldOfView, viewportWidth / viewportHeight, near, far);
}
