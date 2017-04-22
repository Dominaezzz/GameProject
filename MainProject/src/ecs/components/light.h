#pragma once
#include "../component.h"
#include "../../utils/color.h"

enum class LightType
{
	Directional, Point, Spot
};

struct Attenuation
{
	float constant, linear, quadratic;
	Attenuation() : constant(0), linear(0), quadratic(1) {}
	Attenuation(float constant, float linear, float quadratic) : constant(constant), linear(linear), quadratic(quadratic) {}
};

struct Light : Component
{
	LightType lightType = LightType::Directional;
	Color color;// = Color.White;
	float intensity = 0;
	float range = 0;
	Attenuation attenuation;
	float coneAngle;// = MathHelper.PiOver4;
};
