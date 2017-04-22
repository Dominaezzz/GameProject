#pragma once
#include "../../graphics/textures/texture2d.h"
#include "../component.h"

struct Terrain : Component
{
	const static int Size = 800;
	std::shared_ptr<Texture2D> background;
	std::shared_ptr<Texture2D> blendMap;
	std::shared_ptr<Texture2D> textureR, textureG, textureB;
};
