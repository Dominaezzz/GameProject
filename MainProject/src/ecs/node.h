#pragma once
#include "game_object.h"

class Node
{
protected:
	~Node() = default;
private:
	GameObject* gameObject;
public:
	Node() = default;
	virtual void setGameObject(GameObject* gameObject)
	{
		this->gameObject = gameObject;
	}
	GameObject* getGameObject() const { return gameObject; }
};
