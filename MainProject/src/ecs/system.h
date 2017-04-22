#pragma once
#include "world.h"

class System
{
	World* world;
public:
	explicit System(World* world);
	virtual ~System(){}
	virtual void update(float dt){}
	virtual void render(){}
	virtual void onGameObjectEdited(GameObject* gameObject){}
};
