#include "particle_system.h"
#define _USE_MATH_DEFINES
#include <math.h>

ParticleSystem::ParticleSystem(World* world) : System(world)
{
	const Vector2 Vertices[] = { Vector2(0, 0), Vector2(1, 0), Vector2(1, 1), Vector2(0, 1) };
	const int Indexes[] = { 0, 1, 2, 0, 2, 3 };

	particleMesh.setVertexBuffer(0, std::make_shared<VertexBuffer>(Vertices, sizeof(Vertices)), sizeof(Vector2));
	particleMesh.setAttribute<Vector2>(0, VertexAttrib::Position, 0);
	particleMesh.setIndices(Indexes, sizeof(Indexes));
	particleMesh.count = 6;

	particleShader.add(ShaderType::VertexShader, "res/Shaders/Particle/particle.vert");
	particleShader.add(ShaderType::VertexShader, "res/Shaders/Generic/math.glsl");
	particleShader.add(ShaderType::FragmentShader, "res/Shaders/Particle/particle.frag");

	particleShader.compile();

	numberOfRows = particleShader.getUniformLocation("numberOfRows");
#if PARTICLE_SYSTEM_IS_INSTANCED
	particleMesh.setVertexBuffer(1, perParticleBuffer, sizeof(ParticleInstance));
	particleMesh.setAttribute<Vector4>(
		1, particleShader.getAttributeLocation("transform"), offsetof(ParticleInstance, transform), false, true
	);
	particleMesh.setAttribute<float>(
		1, particleShader.getAttributeLocation("indexF"), offsetof(ParticleInstance, indexF), false, true
	);
	particleMesh.setAttribute<float>(
		1, particleShader.getAttributeLocation("blend"), offsetof(ParticleInstance, blend), false, true
	);
#else
	transformation = particleShader.getAttributeLocation("transform");
	index = particleShader.getAttributeLocation("indexF");
	blend = particleShader.getAttributeLocation("blend");
#endif
}

float getValue(float value, float deviation, int power = 1)
{
	double rand = (double(std::rand()) / RAND_MAX);
	if (power != 1) rand = 1 - pow(rand, power);
	return value + static_cast<float>(rand * 2 - 1) * deviation;
}

void init_particle(Particle& particle, ParticleEmitter& emitter, const std::function<int(float)>& size)
{
	Vector3 direction, deviation;
	if (emitter.direction == Vector3(0, 0, 0))
	{
		direction = Vector3(0, 1, 0);
		deviation = Vector3(M_PI);
	}
	else
	{
		direction = emitter.direction;
		deviation = emitter.directionDeviation;
	}

	particle.gravity = getValue(emitter.gravityScale, emitter.gravityScaleDeviation);
	particle.lifeTime = getValue(emitter.lifeTime, emitter.lifeTimeDeviation);
	particle.scale = 1;

	switch (emitter.emitterShape)
	{
	case Source::Point:
		particle.position = Vector3(0);
		break;
	case Source::Line:
		particle.position = Vector3(0, 0, 1) * getValue(0, emitter.emitterSize / 2);
		break;
	case Source::Plane:
		{
			float rand = getValue(0, emitter.emitterSize);
			particle.position = std::rand() % 2 == 0 ? Vector3(size(1), 0, rand) : Vector3(rand, 0, size(1));
			particle.position *= std::rand() % 2 == 0 ? 0.5f : -0.5f;
		}
		break;
	case Source::Circle:
		{
			float theta = getValue(0, M_PI);
			particle.position = Vector3(cos(theta), 0, sin(theta)) * (size(2) * 0.5f);
		}
		break;
	case Source::Sphere:
		particle.position = Quaternion::fromEulerAngles(getValue(0, M_PI), getValue(0, M_PI), getValue(0, M_PI)) *
			Vector3(0, 1, 0) *
			(size(3) * 0.5f);
		break;
	default:
		throw std::invalid_argument("");
	}

	particle.velocity = direction * getValue(emitter.speed, emitter.speedDeviation);
	if (deviation != Vector3(0, 0, 0))
	{
		Quaternion rotation = Quaternion::fromEulerAngles(getValue(0, deviation.x), getValue(0, deviation.y), getValue(0, deviation.z));
		particle.velocity = rotation * particle.velocity;
	}
}

void ParticleSystem::update(float dt)
{
	for (size_t i = 0; i < particleNodes.size(); ++i)
	{
		ParticleNode& node = particleNodes[i];

		auto emitter = node.get<ParticleEmitter>();

		std::function<int(float)> size;
		if (emitter->emitFromEdge)
		{
			size = [emitter](float power) { return emitter->emitterSize; };
		}
		else
		{
			size = [emitter](float power) { return (getValue(0, emitter->emitterSize, power) + emitter->emitterSize) / 2; };
		}

		node.elapsed += emitter->emissionRate * dt;
		for(Particle& particle : node.particles)
		{
			particle.update(dt);

			if (particle.isDead() && node.elapsed > 0)
			{
				init_particle(particle, *emitter, size);
				node.elapsed--;
			}

			// implement emitter.Offset
			float alpha = (particle.lifeTime / emitter->lifeTime) * (emitter->rows * emitter->rows);

			particle.textureIndex = static_cast<size_t>(alpha);
			particle.blend = alpha - particle.textureIndex;
		}

		while (node.elapsed > 0)
		{
			Particle particle;
			init_particle(particle, *emitter, size);
			node.particles.push_back(std::move(particle));
			node.elapsed--;
		}

		node.particles.erase(
			std::remove_if(node.particles.begin(), node.particles.end(), [](const Particle& p)
			{
				return p.isDead();
			}),
			node.particles.end()
		);
	}
}

void ParticleSystem::render()
{
	glEnable(GL_BLEND);
	particleShader.begin();
	particleMesh.bind();

	for (ParticleNode& node : particleNodes)
	{
		auto emitter = node.get<ParticleEmitter>();
		auto transform = node.get<Transform>();
		
		if (emitter->isAdditive)
		{
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			glDepthMask(false);
		}
		else
		{
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glDepthMask(true);

			Vector3 cameraPosition(0);
//			var mainCamera = World.GetSystem<ComponentManager>().GetGameObjectsWith<Camera>().First();
//			{
//				cameraPosition = mainCamera.GetComponent<Transform>().Position;
//			}

			Vector3 camPos = cameraPosition - transform->Position;
			std::sort(node.particles.begin(), node.particles.end(), [camPos](const Particle& left, const Particle& right)
			{
				return (left.position - camPos).lengthSquared() < (right.position - camPos).lengthSquared();
			});
		}

		emitter->texture->bind(0);

		glUniform1i(numberOfRows, emitter->rows);

#if PARTICLE_SYSTEM_IS_INSTANCED
		if (node.particles.size() > instances.size())
		{
			instances.resize(node.particles.size());
			perParticleBuffer->setData(nullptr, node.particles.size() * sizeof(ParticleInstance), BufferUsage::StreamDraw);
		}
		for (size_t index = 0; index < node.particles.size(); ++index)
		{
			const Particle& particle = node.particles[index];
			instances[index].transform = Vector4(transform->Position + particle.position, 3);
			instances[index].indexF = particle.textureIndex;
			instances[index].blend = particle.blend;
		}
		perParticleBuffer->setSubData(instances.data(), 0, node.particles.size() * sizeof(ParticleInstance));

		particleMesh.renderInstanced(node.particles.size(), false);
#else
		for(const auto& particle : node.particles)
		{
			auto temp = transform->Position + particle.position;

			glVertexAttrib4f(transformation, temp.x, temp.y, temp.z, 3);
			glVertexAttrib1f(index, particle.textureIndex);
			glVertexAttrib1f(blend, particle.blend);

			particleMesh.render(false);
		}
#endif

		emitter->texture->unBind(0);
	}

	particleMesh.unBind();
	particleShader.end();
	glDepthMask(true);
}

void ParticleSystem::onGameObjectEdited(GameObject* gameObject)
{
	particleNodes.reportChange(gameObject);
}
