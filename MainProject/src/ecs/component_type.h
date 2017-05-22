#pragma once
#include <stdexcept>
#include <type_traits>
#include "component.h"
#include "components/transform.h"
#include "components/camera.h"
#include "components/light.h"
#include "components/terrain.h"
#include "components/particle_emitter.h"
#include "components/mesh_filter.h"
#include "components/mesh_renderer.h"
#include "components/animator.h"

template<typename ComponentClass>
struct ComponentType
{
	static_assert(std::is_base_of<Component, ComponentClass>(), "Must be a subclass of component");
	constexpr explicit operator int() const
	{
		throw std::invalid_argument("Bit flag not implemented for component type");
	}
};

template<>
struct ComponentType<Transform>
{
	constexpr operator int() const
	{
		return 1 << 0;
	}
};

template<>
struct ComponentType<Camera>
{
	constexpr operator int() const
	{
		return 1 << 1;
	}
};

template<>
struct ComponentType<Light>
{
	constexpr operator int() const
	{
		return 1 << 2;
	}
};

template<>
struct ComponentType<Terrain>
{
	constexpr operator int() const
	{
		return 1 << 3;
	}
};

template<>
struct ComponentType<ParticleEmitter>
{
	constexpr operator int() const
	{
		return 1 << 4;
	}
};

template<>
struct ComponentType<MeshFilter>
{
	constexpr operator int() const
	{
		return 1 << 5;
	}
};

template<>
struct ComponentType<MeshRenderer>
{
	constexpr operator int() const
	{
		return 1 << 6;
	}
};

template<>
struct ComponentType<Animator>
{
	constexpr operator int() const
	{
		return 1 << 7;
	}
};
