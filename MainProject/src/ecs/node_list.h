#pragma once
#include <set>
#include <vector>
#include "game_object.h"
#include <bitset>

template<typename NodeClass>
class NodeList
{
	std::vector<NodeClass> nodes;
	std::bitset<16> required;
	std::bitset<16> reject;
public:
	explicit NodeList(std::bitset<16> required, std::bitset<16> reject = 0);
	bool reportChange(GameObject* object);
	size_t size() { return nodes.size(); }
	NodeClass& operator[](size_t index) { return nodes[index]; }
	const NodeClass& operator[](size_t index) const { return nodes[index]; }
	auto begin() -> decltype(nodes.begin())
	{
		return nodes.begin();
	}
	auto begin() const -> decltype(nodes.begin())
	{
		return nodes.begin();
	}
	auto end() -> decltype(nodes.end())
	{
		return nodes.end();
	}
	auto end() const -> decltype(nodes.end())
	{
		return nodes.end();
	}
};

template<typename NodeClass>
NodeList<NodeClass>::NodeList(std::bitset<16> required, std::bitset<16> reject) : required(required), reject(reject)
{
}

template<typename NodeClass>
bool NodeList<NodeClass>::reportChange(GameObject * object)
{
	auto bitSet = object->getComponentBitSet();
	if((bitSet & required) == required && (bitSet & reject).none())
	{
		auto it = std::find_if(nodes.begin(), nodes.end(), [object](const NodeClass& node) -> bool { return node.getGameObject() == object; });
		if(it == nodes.end())
		{
			NodeClass node;
			node.setGameObject(object);
			nodes.push_back(node);
		}
		else
		{
			it->setGameObject(object);
		}
		return true;
	}
	else
	{
		nodes.erase(
			std::remove_if(nodes.begin(), nodes.end(), [object](const NodeClass& node) -> bool { return node.getGameObject() == object; }),
			nodes.end()
		);
		return false;
	}
}
