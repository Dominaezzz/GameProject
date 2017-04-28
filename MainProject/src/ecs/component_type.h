#pragma once
#include <stdexcept>
#include <type_traits>
#include "component.h"
#include "components/tranform.h"
#include "components/camera.h"
#include "components/light.h"
#include "components/terrain.h"
#include "components/particle_emitter.h"

template<typename ComponentClass>
struct ComponentType
{
	static_assert(std::is_base_of<Component, ComponentClass>(), "Must be a subclass of component");
	explicit operator int() const
	{
		throw std::invalid_argument("Bit flag not implemented for component type");
	}
};

template<>
struct ComponentType<Transform>
{
	operator int() const
	{
		return 1 << 0;
	}
};

template<>
struct ComponentType<Camera>
{
	operator int() const
	{
		return 1 << 1;
	}
};

template<>
struct ComponentType<Light>
{
	operator int() const
	{
		return 1 << 2;
	}
};

template<>
struct ComponentType<Terrain>
{
	operator int() const
	{
		return 1 << 3;
	}
};

template<>
struct ComponentType<ParticleEmitter>
{
	operator int() const
	{
		return 1 << 4;
	}
};