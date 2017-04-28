#pragma once
#include "../node.h"
#include "../components/particle_emitter.h"
#include "../../graphics/mesh.h"
#include "../node_list.h"

#define PARTICLE_SYSTEM_IS_INSTANCED true

struct Particle
{
	Vector3 position;
	Vector3 velocity;
	float gravity = 0;
	float rotation = 0;
	float scale = 0;
	float lifeTime = 0;
	size_t textureIndex = 0;
	float blend = 0;

	bool isDead() const
	{
		return lifeTime <= 0;
	}
	void update(float dt)
	{
		velocity += Vector3(0, -1, 0) * gravity * dt;
		position += velocity * dt;
		lifeTime -= dt;
	}
};

struct ParticleNode final : Node
{
	Transform* transform;
	ParticleEmitter* emitter;

	void setGameObject(GameObject* gameObject) override;

	std::vector<Particle> particles;
	float elapsed = 0;
};

class ParticleSystem : public System
{
	NodeList<ParticleNode> particleNodes = NodeList<ParticleNode>(ComponentType<Transform>() | ComponentType<ParticleEmitter>());
	ShaderProgram particleShader;
	Mesh particleMesh;
#if PARTICLE_SYSTEM_IS_INSTANCED
	std::shared_ptr<VertexBuffer> perParticleBuffer = std::make_shared<VertexBuffer>(0, StreamDraw);

	struct ParticleInstance
	{
		Vector4 transform;
		float indexF = 0;
		float blend = 0;
	};
	std::vector<ParticleInstance> instances;
#else
	GLuint transormation;
	GLuint index;
	GLuint blend;
#endif
	GLuint numberOfRows;

public:
	explicit ParticleSystem(World* world);
	~ParticleSystem() = default;
	void update(float dt) override;
	void render() override;
	void onGameObjectEdited(GameObject* gameObject) override;
};