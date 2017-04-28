#define _USE_MATH_DEFINES

#include <GL/glew.h>
#include <iostream>
#include <math.h>
#include <ctime>

#include "src/io/window.h"
#include "src/graphics/mesh.h"
#include "src/graphics/textures/texture2d.h"
#include "src/math/vector.h"
#include "src/math/matrix.h"
#include "src/ecs/world.h"
#include "src/ecs/systems/camera_system.h"
#include "src/ecs/systems/light_system.h"
#include "src/ecs/systems/terrain_system.h"
#include "src/ecs/systems/particle_system.h"

void old_picture_render(Window& window)
{
	Vector2 positions[] = {
		Vector2(-0.5f, -0.5f),
		Vector2(0.5f, -0.5f),
		Vector2(0.5f,  0.5f),
		Vector2(-0.5f,  0.5f)
	};
	int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	ShaderProgram shader;
	shader.add(ShaderType::VertexShader, "res/shaders/main.vert");
	shader.add(ShaderType::FragmentShader, "res/shaders/main.frag");
	shader.compile();

	Mesh mesh;

	mesh.setVertexAttribute<Vector2>(
		std::make_shared<VertexBuffer>(positions, sizeof(positions), BufferUsage::StaticDraw),
		shader.getAttributeLocation("position"),
		sizeof(Vector2),
		0
		);
	mesh.setIndices(indices, 6 * sizeof(int));

	std::shared_ptr<Texture2D> image = Texture2D::fromFile("res/images/Cover.jpg");

	glClearColor(0.2f, 0.2f, 1.0f, 1);

	clock_t start = clock();
	clock_t timer = 0;
	while (!window.shouldClose())
	{
		window.pollEvents();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.begin();
		{
			image->bind(0);
			mesh.render(GL_TRIANGLES);
			//  mesh.renderInstanced(shader, GL_TRIANGLES, 1000);
			//  for (size_t i = 0; i < 1000; i++)
			//  {
			//  	mesh.render(shader, GL_TRIANGLES);
			//  }
		}
		shader.end();

		window.swapBuffers();

		clock_t period = clock() - start;
		timer += period;
		if (timer > CLOCKS_PER_SEC)
		{
			std::cout << (double(CLOCKS_PER_SEC) / period) << " FPS" << std::endl;
			timer -= CLOCKS_PER_SEC;
		}
		start = clock();
	}
}

void setUpLights(World& world)
{
	GameObject* sun = world.newGameObject();
	{
		Transform* transform = sun->addComponent<Transform>();
		transform->Rotation = Quaternion::fromAxisAngle(Vector3(1, 0, 0), float(M_PI) * 7 / 6);

		auto lightSource = sun->addComponent<Light>();
		lightSource->lightType = LightType::Directional;
		lightSource->color = Color::White;
		lightSource->intensity = 0.5f;
	}

	GameObject* lamp = world.newGameObject();
	{
		auto transform = lamp->addComponent<Transform>();
		transform->Position = 5.0f * Vector3(0, 1, 0);
		transform->Position += -(Vector3(1, 0, 1)) * 300.0f;

		auto lightSource = lamp->addComponent<Light>();
		lightSource->lightType = LightType::Point;
		lightSource->color = Color::White;
		lightSource->intensity = 50.0f;
		lightSource->attenuation = Attenuation(0, 1, 0);
		lightSource->range = 500;// CalculateRange(lightSource.Attenuation, lightSource.Color, lightSource.Intensity);
	}

	GameObject* torchLight = world.newGameObject();
	{
		auto transform = torchLight->addComponent<Transform>();
		transform->Position = 2.0f * Vector3(0, 1, 0);
//		transform->Rotation = Quaternion::fromEulerAngles(0, MathHelper.PiOver4, -2 * MathHelper.PiOver4);
		transform->Rotation = Quaternion::fromAxisAngle(Vector3(0, 1, 0), float(M_PI_4)) * Quaternion::fromAxisAngle(Vector3(0, 0, 1), -2 * float(M_PI_4));

		auto lightSource = torchLight->addComponent<Light>();
		lightSource->lightType = LightType::Spot;
		lightSource->color = Color::White;
		lightSource->intensity = 30.0f;
		lightSource->attenuation = Attenuation(0, 1, 0);
		lightSource->range = 100;// CalculateRange(lightSource.Attenuation, lightSource.Color, lightSource.Intensity);
		lightSource->coneAngle = float(M_PI_4);
	}
}

void setUpParticleEffects(World& world)
{
	GameObject* fire = world.newGameObject();
	{
		auto tranform = fire->addComponent<Transform>();
		tranform->Position = Vector3(-150, 0, 100);

		auto particleEmitter = fire->addComponent<ParticleEmitter>();
		particleEmitter->emitterShape = Source::Circle;
		particleEmitter->emitterSize = 200;
		particleEmitter->texture = Texture2D::fromFile("res/Particle/fire.png");
		particleEmitter->rows = 8;
		particleEmitter->offset = 0;
		
		particleEmitter->isAdditive = true;
		particleEmitter->emissionRate = 1000;
		particleEmitter->speed = 5;
		particleEmitter->speedDeviation = 1;
		particleEmitter->lifeTime = 10;// 3f;

		particleEmitter->direction = Vector3(0, 1, 0);
		//particleEmitter.DirectionDeviation = new Vector3(MathHelper.PiOver6);
	}

//	GameObject* magic = world.newGameObject();
//	{
//		auto transform = magic->addComponent<Transform>();
//		transform->Position = Vector3(-Terrain::Size / 2.0f, 0, -Terrain::Size / 2.0f);// -60);
//
//		auto particleEmitter = magic->addComponent<ParticleEmitter>();
//		particleEmitter->emitterShape = Source::Plane;
//		particleEmitter->emitterSize = Terrain::Size;
//		particleEmitter->texture = Texture2D::fromFile("Content/Particle/cosmic.png");
//		particleEmitter->rows = 4;
//		particleEmitter->offset = 0;
//
//		//particleEmitter.IsAdditive = true;
//		particleEmitter->emissionRate = 1000;
//		particleEmitter->speed = 10;
//		particleEmitter->speedDeviation = 1;
//		particleEmitter->lifeTime = 1;// 3f;
//
//		particleEmitter->direction = Vector3(0, 1, 0);
//	}
//
//	GameObject* stars = world.newGameObject();
//	{
//		auto transform = stars->addComponent<Transform>();
//		transform->Position = Vector3(-Terrain::Size / 2.0f, 200, -Terrain::Size / 2.0f);
//
//		auto particleEmitter = stars->addComponent<ParticleEmitter>();
//		particleEmitter->emitterShape = Source::Point;
//		particleEmitter->texture = Texture2D::fromFile("Content/Particle/particleStar.png");
//		particleEmitter->rows = 1;
//		particleEmitter->offset = 0;
//
//		particleEmitter->isAdditive = true;
//		particleEmitter->emissionRate = 1000;
//		particleEmitter->speed = 50;
//		particleEmitter->speedDeviation = 1;
//		particleEmitter->lifeTime = 1;
//	}
//
//	GameObject* atlas = world.newGameObject();
//	{
//		auto transform = atlas->addComponent<Transform>();
//		transform->Position = Vector3(0, 0, -Terrain::Size / 2.0f);
//
//		auto particleEmitter = atlas->addComponent<ParticleEmitter>();
//		particleEmitter->emitterShape = Source::Line;
//		particleEmitter->emitterSize = Terrain::Size;
//		particleEmitter->texture = Texture2D::fromFile("Content/Particle/particleAtlas.png");
//		particleEmitter->rows = 4;
//		particleEmitter->offset = 0;
//
//		particleEmitter->isAdditive = true;
//		particleEmitter->emissionRate = 1000;
//		particleEmitter->speed = 5;
//		particleEmitter->speedDeviation = 1;
//		particleEmitter->lifeTime = 10;// 3f;
//
//		particleEmitter->direction = Vector3(0, 1, 0);
//		//particleEmitter.DirectionDeviation = new Vector3(MathHelper.PiOver6);
//	}
}

int main() {

	Window window("Main Window", 1080, 720);
	window.setSwapInterval(0);

	std::cout << "OpenGL " << glGetString(GL_VERSION) << std::endl << std::endl;

	//old_picture_render(window);

	glEnable(GL_CULL_FACE);
//	glEnable(GL_ALPHA_TEST);
	glEnable(GL_DEPTH_CLAMP);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	World world;
	world.addSystem<CameraSystem>();
	world.addSystem<LightSystem>();
	world.addSystem<TerrainSystem>();
	world.addSystem<ParticleSystem>();

	auto worldCamera = world.newGameObject();
	auto camTransform = worldCamera->addComponent<Transform>();
	camTransform->Position = Vector3(0, 10, 0);
	camTransform->Rotation = Quaternion::fromAxisAngle(Vector3(0, 1, 0), 0);

	auto camera = worldCamera->addComponent<Camera>();
	auto temp = TextureCube::fromFile(
		"res/SkyBox/right.png",
		"res/SkyBox/left.png",
		"res/SkyBox/top.png",
		"res/SkyBox/bottom.png",
		"res/SkyBox/back.png",
		"res/SkyBox/front.png"
	);
	camera->skyBox = temp.get();
	camera->fieldOfView = 70 * (M_PI / 180.0F);
	camera->near = 0.1f;
	camera->far = 1000.0f;
	camera->viewportWidth = window.getWidth();
	camera->viewportHeight = window.getHeight();

	setUpLights(world);
	setUpParticleEffects(world);

	auto ground = world.newGameObject();
	{
		auto transform = ground->addComponent<Transform>();
		transform->Position = Vector3(0) * float(Terrain::Size);

		auto terrainComponent = ground->addComponent<Terrain>();
		terrainComponent->background = Texture2D::fromFile("res/Terrain/grassy2.png", true);
		terrainComponent->blendMap = Texture2D::fromFile("res/Terrain/blendMap.png", true);
		terrainComponent->textureR = Texture2D::fromFile("res/Terrain/mud.png", true);
		terrainComponent->textureG = Texture2D::fromFile("res/Terrain/grassFlowers.png", true);
		terrainComponent->textureB = Texture2D::fromFile("res/Terrain/path.png", true);
	}

	clock_t start = clock();
	clock_t timer = 0;
	while (!window.shouldClose())
	{
		clock_t period = clock() - start;
		timer += period;
		if (timer > CLOCKS_PER_SEC)
		{
			std::cout << (double(CLOCKS_PER_SEC) / period) << " FPS" << std::endl;
			timer -= CLOCKS_PER_SEC;
		}
		start = clock();

		window.pollEvents();

		//glClearColor(0.2f, 0.2f, 1.0f, 1);
		glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		float dt = float(period) / float(CLOCKS_PER_SEC);

		if(Keyboard::isKeyDown(Key::UP))
		{
			camTransform->Rotation = camTransform->Rotation * Quaternion::fromAxisAngle(Vector3(1, 0, 0), 100 * (M_PI / 180.0F) * dt);
		}
		if(Keyboard::isKeyDown(Key::DOWN))
		{
			camTransform->Rotation = camTransform->Rotation * Quaternion::fromAxisAngle(Vector3(1, 0, 0), -100 * (M_PI / 180.0F) * dt);
		}
		if (Keyboard::isKeyDown(Key::LEFT))
		{
			camTransform->Rotation = Quaternion::fromAxisAngle(Vector3(0, 1, 0), 100 * (M_PI / 180.0F) * dt) * camTransform->Rotation;
		}
		if (Keyboard::isKeyDown(Key::RIGHT))
		{
			camTransform->Rotation = Quaternion::fromAxisAngle(Vector3(0, 1, 0), -100 * (M_PI / 180.0F) * dt) * camTransform->Rotation;
		}
		if (Keyboard::isKeyDown(Key::SPACE))
		{
			camTransform->Position += (camTransform->Rotation * Vector3(0, 0, -1)) * 100.0F * dt;
		}
		if (Keyboard::isKeyDown(Key::R))
		{
			camTransform->Position = Vector3(0, 0, 0);
		}

		world.update(dt);
		world.render();

		window.swapBuffers();
	}

	return 0;
}