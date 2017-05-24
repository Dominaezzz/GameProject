#pragma once
#include <map>
#include <memory>
#include <typeinfo>
#include <vector>
#include <unordered_map>
#include <typeindex>

class System;
class GameObject;

class World
{
	std::vector<std::unique_ptr<System>> systems;
	std::unordered_map<std::type_index, System*> systemsMap;
	std::vector<std::unique_ptr<GameObject>> gameObjects;
public:
	World();
	void update(float dt);
	void render();
	GameObject* newGameObject();
	template<typename SystemClass> SystemClass* addSystem();
	template<typename SystemClass> SystemClass* getSystem() const;
	template<typename SystemClass> bool hasSystem() const;
	template<typename SystemClass> void removeSystem();

	void onGameObjectComponentAdded(GameObject* gameObject);
	void onGameObjectComponentRemoved(GameObject* gameObject);
};

#include "system.h"
#include "game_object.h"

template<typename SystemClass>
SystemClass* World::addSystem()
{
	std::unique_ptr<SystemClass> system = std::make_unique<SystemClass>(this);
	SystemClass* systemPtr = system.get();
	systemsMap[std::type_index(typeid(SystemClass))] = systemPtr;
	systems.push_back(std::move(system));
	return systemPtr;
}

template<typename SystemClass>
SystemClass* World::getSystem() const
{
	return static_cast<SystemClass*>(systemsMap.at(std::type_index(typeid(SystemClass))));
}

template<typename SystemClass>
bool World::hasSystem() const
{
	return systemsMap.count(std::type_index(typeid(SystemClass))) > 0;
}

template <typename SystemClass>
void World::removeSystem()
{
	auto temp = systemsMap.find(std::type_index(typeid(SystemClass)));
	if(temp != systemsMap.end())
	{
		systems.erase(std::remove_if(systems.begin(), systems.end(), [temp](const std::unique_ptr<System>& item)
		{
			return item.get() == temp->second;
		}), systems.end());
	}
}
