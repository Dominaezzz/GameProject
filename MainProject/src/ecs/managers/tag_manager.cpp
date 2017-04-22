#include "tag_manager.h"

inline void TagManager::tag(GameObject* gameObject, std::string tag)
{
	objectsByTags[tag] = gameObject;
	tagsByObjects[gameObject] = tag;
}

inline void TagManager::unTag(GameObject* gameObject, std::string tag)
{
	auto it = tagsByObjects.find(gameObject);
	if(it != tagsByObjects.end())
	{
		tagsByObjects.erase(it);

		auto it2 = objectsByTags.find(tag);
		if(it2 != objectsByTags.end())
		{
			objectsByTags.erase(it2);
		}
	}
}

inline std::string TagManager::getTag(GameObject* gameObject) const
{
	return tagsByObjects.at(gameObject);
}

inline GameObject* TagManager::getGameObject(std::string tag) const
{
	return objectsByTags.at(tag);
}