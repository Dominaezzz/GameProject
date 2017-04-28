#pragma once
#include "../component.h"
#include "../../math/vector.h"
#include "../../graphics/textures/texture2d.h"

enum class Source
{
	Point, Line, Plane, Circle, Sphere
};

struct ParticleEmitter : Component
{
	Source emitterShape = Source::Point;
	float emitterSize = 0;
	bool emitFromEdge = false;
	size_t emissionRate = 0;

	std::shared_ptr<Texture2D> texture;
	bool isAdditive = false;
	size_t offset = 0;
	size_t rows = 0;

	float speed = 0;
	float speedDeviation = 0;

	Vector3 direction = Vector3(0);
	Vector3 directionDeviation = Vector3(0);

	float gravityScale = 0;
	float gravityScaleDeviation = 0;

	float lifeTime = 0;
	float lifeTimeDeviation = 0;
};
