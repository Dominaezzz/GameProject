#pragma once
#include <unordered_map>
#include "../game_object.h"

class TagManager
{
	std::unordered_map<std::string, GameObject*> objectsByTags;
	std::unordered_map<GameObject*, std::string> tagsByObjects;
public:
	void tag(GameObject* gameObject, std::string tag);
	void unTag(GameObject* gameObject, std::string tag);
	std::string getTag(GameObject* gameObject) const;
	GameObject* getGameObject(std::string tag) const;
};