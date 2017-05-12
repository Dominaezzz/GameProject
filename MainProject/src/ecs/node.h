#pragma once
#include "game_object.h"

class INode
{
protected:
	~INode() = default;
private:
	GameObject* gameObject;
public:
	INode() = default;
	virtual void setGameObject(GameObject* gameObject)
	{
		this->gameObject = gameObject;
	}
	GameObject* getGameObject() const { return gameObject; }
};

template<typename ...Components>
class Node : public INode
{
public:
	virtual ~Node() = default;
	std::tuple<Components*...> components;

	template <typename T>
	T* get()
	{
		return std::get<T*>(components);
	}

	template<typename T1>
	inline void setComponent()
	{
		std::get<T1*>(components) = getGameObject()->getComponent<T1>();
	}

	template<typename T1, typename T2, typename ...Tn>
	inline void setComponent()
	{
		setComponent<T1>();
		setComponent<T2, Tn...>();
	}

	void setGameObject(GameObject* gameObject) override
	{
		INode::setGameObject(gameObject);
		setComponent<Components...>();
	}
};