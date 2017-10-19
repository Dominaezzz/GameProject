#define _USE_MATH_DEFINES

#include <GL/glew.h>
#include <iostream>
#include <math.h>
#include <ctime>

#include "src/io/window.h"
#include "src/graphics/mesh.h"
#include "src/graphics/textures/texture2d.h"
#include "src/math/vector.h"
#include "src/ecs/world.h"
#include "src/ecs/systems/camera_system.h"
#include "src/ecs/systems/light_system.h"
#include "src/ecs/systems/terrain_system.h"
#include "src/ecs/systems/particle_system.h"
#include "src/utils/importer.h"
#include "src/ecs/systems/mesh_rendering_system.h"
#include "src/ecs/systems/animation_system.h"

void setUpGround(World& world)
{
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
		transform->Position = Vector3(-300, 5, -300);

		auto lightSource = lamp->addComponent<Light>();
		lightSource->lightType = LightType::Point;
		lightSource->color = Color::White;
		lightSource->intensity = 50.0f;
		lightSource->attenuation = Attenuation(0, 1, 0);
		lightSource->range = 100;
	}

	GameObject* modelLight = world.newGameObject();
	{
		auto transform = modelLight->addComponent<Transform>();
		transform->Position = Vector3(-100, 10, -95);

		auto lightSource = modelLight->addComponent<Light>();
		lightSource->lightType = LightType::Point;
		lightSource->color = Color::White;
		lightSource->intensity = 40.0f;
		lightSource->attenuation = Attenuation(0, 1, 0);
		lightSource->range = 200;
	}

	GameObject* torchLight = world.newGameObject();
	{
		auto transform = torchLight->addComponent<Transform>();
		transform->Position = 2.0f * Vector3(0, 1, 0);
		transform->Rotation = Quaternion::fromEulerAngles(0, float(M_PI_4), -2 * float(M_PI_4));

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

GameObject* buildBox(World& world)
{
	GameObject* box = world.newGameObject();

	Transform* transform = box->addComponent<Transform>();
	{
		transform->Position += Vector3(-40, 15, -40);
		transform->Scale = Vector3(10);
	}

	MeshFilter* filter = box->addComponent<MeshFilter>();
	{
		const int indices[] =
		{
			0, 1, 2,  2, 3, 0,
			4, 1, 0,  0, 5, 4,
			2, 6, 7,  7, 3, 2,
			4, 5, 7,  7, 6, 4,
			0, 3, 7,  7, 5, 0,
			1, 4, 2,  2, 4, 6
		};

		const float Size = 1;
		const Vector3 vertices[] = {
			Vector3(-Size,  Size, -Size), Vector3(-Size, -Size, -Size),
			Vector3(Size, -Size, -Size), Vector3(Size,  Size, -Size),

			Vector3(-Size, -Size,  Size), Vector3(-Size,  Size,  Size),
			Vector3(Size, -Size,  Size), Vector3(Size,  Size,  Size)
		};

		filter->mesh = std::make_shared<Mesh>();
		filter->mesh->setVertexBuffer(0, std::make_shared<VertexBuffer>(vertices, sizeof(vertices)), sizeof(Vector3));
		filter->mesh->setAttribute<Vector3>(0, VertexAttrib::Position, 0);
		filter->mesh->setIndices(indices, sizeof(indices));
	}

	MeshRenderer* renderer = box->addComponent<MeshRenderer>();
	{
		renderer->setColor(MaterialProperty::ColorAmbient, Color::Green);
		renderer->setColor(MaterialProperty::ColorDiffuse, Color::Red);
//		renderer->setTexture(MaterialProperty::TextureDiffuse, Texture2D::fromFile("res/images/Cover.jpg"));
	}

	return box;
}

void setUpModels(World& world)
{
	GameObject* box = Importer::load(world, "res/Models/Box.gltf");
	{
		auto t = box->getComponent<Transform>();
		t->Position = Vector3(-50, 10, -10);
		t->Scale = Vector3(5);
	}

	GameObject* boxTextured = Importer::load(world, "res/Models/BoxTextured.gltf");
	{
		auto t = boxTextured->getComponent<Transform>();
		t->Position = Vector3(-50, 10, -30);
		t->Scale = Vector3(5);
	}

	GameObject* duck = Importer::load(world, "res/Models/Duck.gltf");
	{
		auto t = duck->getComponent<Transform>();
		t->Position = Vector3(-50, 10, -50);
		t->Scale = Vector3(0.05f);
	}

	GameObject* face = Importer::load(world, "res/Models/SmilingFace.gltf");
	{
		auto t = face->getComponent<Transform>();
		t->Position = Vector3(-50, 10, -70);
		t->Scale = Vector3(5);
	}

//	GameObject* engine = Importer::load(world, "res/Models/2CylinderEngine.gltf");
//	{
//		auto t = engine->getComponent<Transform>();
//		t->Position = Vector3(-50, 10, -90);
//		t->Scale = Vector3(0.05f);
//	}

	GameObject* boxAnim = Importer::load(world, "res/Models/BoxAnimated.gltf");
	{
		auto t = boxAnim->getComponent<Transform>();
		t->Position = Vector3(-30, 10, -10);
		t->Scale = Vector3(5);
	}
}

void APIENTRY openglCallbackFunction(GLenum source, GLenum type, GLuint id, GLenum severity,
	GLsizei length, const GLchar* message, const void* userParam) {
	using std::cout;
	using std::endl;
	if(type != GL_DEBUG_TYPE_ERROR)
	{
		return;
	}
	std::cout << "---------------------opengl-callback-start------------" << std::endl;
	cout << "message: " << message << endl;
	cout << "type: ";
	switch (type) {
	case GL_DEBUG_TYPE_ERROR:
		cout << "ERROR";
		break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		cout << "DEPRECATED_BEHAVIOR";
		break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		cout << "UNDEFINED_BEHAVIOR";
		break;
	case GL_DEBUG_TYPE_PORTABILITY:
		cout << "PORTABILITY";
		break;
	case GL_DEBUG_TYPE_PERFORMANCE:
		cout << "PERFORMANCE";
		break;
	case GL_DEBUG_TYPE_OTHER:
		cout << "OTHER";
		break;
	}
	cout << endl;

	cout << "id: " << id << endl;
	cout << "severity: ";
	switch (severity) {
	case GL_DEBUG_SEVERITY_LOW:
		cout << "LOW";
		break;
	case GL_DEBUG_SEVERITY_MEDIUM:
		cout << "MEDIUM";
		break;
	case GL_DEBUG_SEVERITY_HIGH:
		cout << "HIGH";
		break;
	}
	cout << endl;
	cout << "---------------------opengl-callback-end--------------" << endl;
}

int main() {

	Window window("Main Window", 1080, 720);
	window.setSwapInterval(0);

	std::cout << "OpenGL " << glGetString(GL_VERSION) << std::endl << std::endl;
	if (glDebugMessageCallback) {
		std::cout << "Register OpenGL debug callback " << std::endl;
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(openglCallbackFunction, nullptr);
		GLuint unusedIds = 0;
		glDebugMessageControl(GL_DEBUG_SOURCE_API, GL_DEBUG_TYPE_ERROR, GL_DEBUG_SEVERITY_HIGH, 0, &unusedIds, true);
	}
	else
		std::cout << "glDebugMessageCallback not available" << std::endl;

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
	world.addSystem<AnimationSystem>();
	world.addSystem<MeshRenderingSystem>();

	auto worldCamera = world.newGameObject();
	auto camTransform = worldCamera->addComponent<Transform>();
	camTransform->Position = Vector3(0, 10, 0);
	camTransform->Rotation = Quaternion::fromAxisAngle(Vector3(0, 1, 0), 0);

	auto camera = worldCamera->addComponent<Camera>();
	auto temp = TextureCube::fromFile(
		"res/SkyBox/right.png", "res/SkyBox/left.png", 
		"res/SkyBox/top.png", "res/SkyBox/bottom.png",
		"res/SkyBox/back.png", "res/SkyBox/front.png"
	);
	camera->skyBox = temp.get();
	camera->FieldOfView = static_cast<float>(70 * (M_PI / 180.0F));
	camera->Near = 0.1f;
	camera->Far = 1000.0f;
	camera->ViewportWidth = static_cast<float>(window.getWidth());
	camera->ViewportHeight = static_cast<float>(window.getHeight());

	setUpGround(world);
	setUpLights(world);
	setUpParticleEffects(world);
	setUpModels(world);

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
