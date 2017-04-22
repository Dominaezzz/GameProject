#pragma once
#include "../component.h"
#include "../../math/matrix.h"
#include "../../graphics/textures/texture_cube.h"

struct Camera : Component
{
	float fieldOfView;
	float near;
	float far;
	float viewportWidth;
	float viewportHeight;

	TextureCube* skyBox;

	Matrix4 getProjection();
};
