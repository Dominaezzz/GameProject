#include "director.h"

Director::Director()
{
}

Director::~Director()
{
	while (!sceneStack.empty())
	{
		popCurrentScene();
	}
}

Scene * Director::getCurrentScene()
{
	if (!sceneStack.empty())
	{
		return sceneStack.top();
	}
	return nullptr;
}

bool Director::hasCurrentScene()
{
	return !sceneStack.empty();
}

void Director::popCurrentScene()
{
	Scene* scene = sceneStack.top();
	sceneStack.pop();
	delete scene;
}
