#pragma once
#include "../component.h"
#include "../../graphics/mesh.h"
#include "../../math/matrix.h"

struct MeshFilter : Component
{
	std::shared_ptr<Mesh> mesh;
	std::vector<Matrix4> bindPoses = {};
};
