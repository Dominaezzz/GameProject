#pragma once
#include "../system.h"
#include "../node.h"
#include "../node_list.h"

class AnimationSystem : public System
{
	class AnimationNode : public Node<Transform, Animator>
	{
	public:
		float elapsedTime = 0;
	};

	NodeList<AnimationNode> nodes = NodeList<AnimationNode>(ComponentType<Transform>() | ComponentType<Animator>());
public:
	explicit AnimationSystem(World* world);
	~AnimationSystem() override;
	void update(float dt) override;
	void render() override;
	void onGameObjectEdited(GameObject* gameObject) override;
};
