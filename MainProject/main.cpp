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
		GL_FLOAT,
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
		transform->rotation = Quaternion::fromAxisAngle(Vector3(1, 0, 0), M_PI + (M_PI / 6));

		auto lightSource = sun->addComponent<Light>();
		lightSource->lightType = LightType::Directional;
		lightSource->color = Color(1, 1, 1);
		lightSource->intensity = 0.5f;
	}

	GameObject* lamp = world.newGameObject();
	{
		auto transform = lamp->addComponent<Transform>();
		transform->position = 5.0f * Vector3(0, 1, 0);
		transform->position += -(Vector3(1, 0, 1)) * 300.0f;

		auto lightSource = lamp->addComponent<Light>();
		lightSource->lightType = LightType::Point;
		lightSource->color = Color(1, 1, 1);
		lightSource->intensity = 50.0f;
		lightSource->attenuation = Attenuation(0, 1, 0);
		lightSource->range = 500;// CalculateRange(lightSource.Attenuation, lightSource.Color, lightSource.Intensity);
	}

	GameObject* torchLight = world.newGameObject();
	{
		auto transform = torchLight->addComponent<Transform>();
		transform->position = 2.0f * Vector3(0, 1, 0);
//		transform->rotation = Quaternion::fromEulerAngles(0, MathHelper.PiOver4, -2 * MathHelper.PiOver4);
		transform->rotation = Quaternion::fromAxisAngle(Vector3(0, 1, 0), M_PI / 4) * Quaternion::fromAxisAngle(Vector3(0, 0, 1), -2 * M_PI / 4);

		auto lightSource = torchLight->addComponent<Light>();
		lightSource->lightType = LightType::Spot;
		lightSource->color = Color(1, 1, 1);
		lightSource->intensity = 30.0f;
		lightSource->attenuation = Attenuation(0, 1, 0);
		lightSource->range = 100;// CalculateRange(lightSource.Attenuation, lightSource.Color, lightSource.Intensity);
		lightSource->coneAngle = M_PI / 4;
	}
}

int main() {

	Window window("Main Window", 1080, 720);

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

	auto worldCamera = world.newGameObject();
	auto camTransform = worldCamera->addComponent<Transform>();
	camTransform->position = Vector3(0, 10, 0);
	camTransform->rotation = Quaternionf::fromAxisAngle(Vector3(0, 1, 0), 0);

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

	auto ground = world.newGameObject();
	{
		auto transform = ground->addComponent<Transform>();
		transform->position = Vector3(0) * float(Terrain::Size);

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
			camTransform->rotation = camTransform->rotation * Quaternion::fromAxisAngle(Vector3(1, 0, 0), 100 * (M_PI / 180.0F) * dt);
		}
		if(Keyboard::isKeyDown(Key::DOWN))
		{
			camTransform->rotation = camTransform->rotation * Quaternion::fromAxisAngle(Vector3(1, 0, 0), -100 * (M_PI / 180.0F) * dt);
		}
		if (Keyboard::isKeyDown(Key::LEFT))
		{
			camTransform->rotation = Quaternion::fromAxisAngle(Vector3(0, 1, 0), 100 * (M_PI / 180.0F) * dt) * camTransform->rotation;
		}
		if (Keyboard::isKeyDown(Key::RIGHT))
		{
			camTransform->rotation = Quaternion::fromAxisAngle(Vector3(0, 1, 0), -100 * (M_PI / 180.0F) * dt) * camTransform->rotation;
		}
		if (Keyboard::isKeyDown(Key::SPACE))
		{
			camTransform->position += (camTransform->rotation * Vector3(0, 0, -1)) * 100.0F * dt;
		}
		if (Keyboard::isKeyDown(Key::R))
		{
			camTransform->position = Vector3(0, 0, 0);
		}

		world.update(dt);
		world.render();

		window.swapBuffers();
	}

	return 0;
}