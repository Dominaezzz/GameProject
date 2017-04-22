#pragma once

#include <GL\glew.h>

class Scene
{
public:
	Scene();
	~Scene();
	virtual void update(float dt) = 0;
	virtual void render() = 0;
	virtual void resize(int width, int height) {};
private:

};
