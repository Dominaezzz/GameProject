#pragma once
#include "../ecs/game_object.h"

class Importer
{
public:
	static GameObject* load(World& world, const std::string& path);
};
