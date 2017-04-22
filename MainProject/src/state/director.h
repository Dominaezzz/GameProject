#pragma once

#include <stack>
#include "scene.h"

class Director
{
private:
	std::stack<Scene*> sceneStack;
public:
	Director();
	~Director();

	template<typename S>
	void pushScene()
	{
		static_assert(std::is_base_of<Scene, S>(), "S should be of type 'Scene'");
		
		Scene* scene = new S();
		sceneStack.push(scene);
	}
	Scene* getCurrentScene();
	bool hasCurrentScene();
	void popCurrentScene();

};
