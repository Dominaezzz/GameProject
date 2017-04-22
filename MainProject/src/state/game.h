#pragma once

#include "director.h"
#include "../input/window.h"

class Game
{
private:
	input::Window window;
	Director director;
public:
	Game();
	~Game();
	virtual void update(float dt);
	virtual void render();
	virtual void resize(int width, int height) {};
	void run();
};

Game::Game() : window("Seth Stalin", 1080, 720)
{
}

Game::~Game()
{
}

void Game::update(float dt)
{
	Scene* scene = director.getCurrentScene();
	if (scene)
	{
		scene->update(dt);
	}
}

void Game::render()
{
	Scene* scene = director.getCurrentScene();
	if (scene)
	{
		scene->render();
	}
}

void Game::run()
{
	while (!window.shouldClose())
	{
		update(1 / 60.0f);
		render();
	}
}
