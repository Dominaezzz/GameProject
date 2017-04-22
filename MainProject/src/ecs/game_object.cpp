#include "game_object.h"

GameObject::GameObject(World * world) : world(world)
{
}

size_t GameObject::getComponentCount() const
{
	return size_t(components.size());
}

std::bitset<16> GameObject::getComponentBitSet() const
{
	return componentBitSet;
}
