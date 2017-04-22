#include "world.h"

World::World()
{
}

void World::update(float dt)
{
	for (auto& system : systems)
	{
		system->update(dt);
	}
}

void World::render()
{
	for (auto& system : systems)
	{
		system->render();
	}
}

GameObject * World::newGameObject()
{
	auto object = new GameObject(this);
	gameObjects.push_back(std::unique_ptr<GameObject>(object));
	return object;
}

void World::onGameObjectComponentAdded(GameObject * gameObject)
{
	for (auto& system : systems)
	{
		system->onGameObjectEdited(gameObject);
	}
}

void World::onGameObjectComponentRemoved(GameObject * gameObject)
{
	for (auto& system : systems)
	{
		system->onGameObjectEdited(gameObject);
	}
}
