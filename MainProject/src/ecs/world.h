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
//	std::unordered_map<std::type_index, std::unique_ptr<System>> systems;
	std::vector<std::unique_ptr<System>> systems;
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
//	systems[typeid(SystemClass)] = system;

//	systems[std::type_index(typeid(SystemClass))] = 
	systems.push_back(std::move(system));
	return systemPtr;
}

template<typename SystemClass>
SystemClass* World::getSystem() const
{
	return nullptr; //systems.at(std::type_index(typeid(SystemClass))).get();
}

template<typename SystemClass>
bool World::hasSystem() const
{
	return false;// systems.count(std::type_index(typeid(SystemClass))) == 1;
}

template <typename SystemClass>
void World::removeSystem()
{
//	auto temp = systems.find(std::type_index(typeid(SystemClass)));
//	if(temp != systems.end())
//	{
//		systems.erase(temp);
//	}
}
