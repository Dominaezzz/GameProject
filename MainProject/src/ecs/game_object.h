#pragma once
#include <bitset>
#include <map>
#include <memory>
#include "component.h"
#include "component_type.h"
#include "world.h"

class GameObject
{
	World* world;
	std::map<std::type_index, std::unique_ptr<Component>> components;
	std::bitset<16> componentBitSet;
public:
	explicit GameObject(World* world);

	template<typename ComponentClass>
	ComponentClass* addComponent();

	template<typename ComponentClass>
	ComponentClass* getComponent() const;

	template<typename ComponentClass>
	bool hasComponent() const;

	template<typename ComponentClass>
	void removeComponent();

	size_t getComponentCount() const;

	std::bitset<16> getComponentBitSet() const;
};

template <typename ComponentClass>
ComponentClass* GameObject::addComponent()
{
	ComponentClass* component = new ComponentClass();
	components[std::type_index(typeid(ComponentClass))] = std::unique_ptr<ComponentClass>(component);
	componentBitSet |= int(ComponentType<ComponentClass>());
	world->onGameObjectComponentAdded(this);
	return component;
}

template <typename ComponentClass>
ComponentClass* GameObject::getComponent() const
{
	return static_cast<ComponentClass*>(components.at(std::type_index(typeid(ComponentClass))).get());
}

template <typename ComponentClass>
bool GameObject::hasComponent() const
{
	return components.count(std::type_index(typeid(ComponentClass))) == 1;
}

template <typename ComponentClass>
void GameObject::removeComponent()
{
	auto temp = components.find(std::type_index(typeid(ComponentClass)));
	if (temp != components.end())
	{
		components.erase(temp);
		componentBitSet &= ~int(ComponentType<ComponentClass>());
		world->onGameObjectComponentRemoved(this);
	}
}