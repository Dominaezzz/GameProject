#pragma once
#include <map>
#include <vector>
#include "../component.h"
#include "transform.h"
#include "../../graphics/textures/texture2d.h"
#include "../../utils/color.h"

enum class MaterialProperty : size_t
{
	TextureAmbient    = 1 << 0,
	TextureDiffuse    = 1 << 1,
	TextureSpecular   = 1 << 2,
	TextureNormal     = 1 << 3,
	TextureEmissive   = 1 << 4,
	TextureReflective = 1 << 5,

	ColorAmbient      = 1 << 6,
	ColorDiffuse      = 1 << 7,
	ColorSpecular     = 1 << 8,
	ColorNormal       = 1 << 9,
	ColorEmissive     = 1 << 10,
	ColorReflective   = 1 << 11,

	FloatReflectivity = 1 << 12,
	FloatShininess    = 1 << 13
};

inline constexpr MaterialProperty operator|(MaterialProperty left, MaterialProperty right)
{
	return static_cast<MaterialProperty>(static_cast<size_t>(left) | static_cast<size_t>(right));
}
inline constexpr MaterialProperty operator&(MaterialProperty left, MaterialProperty right)
{
	return static_cast<MaterialProperty>(static_cast<size_t>(left) & static_cast<size_t>(right));
}
inline constexpr MaterialProperty operator|=(MaterialProperty& left, MaterialProperty right)
{
	left = left | right;
	return left;
}
inline constexpr MaterialProperty operator&=(MaterialProperty& left, MaterialProperty right)
{
	left = left & right;
	return left;
}

class Material final
{
	std::map<MaterialProperty, std::shared_ptr<Texture2D>> textures;
	std::map<MaterialProperty, Color> colors;
	std::map<MaterialProperty, float> floats;
	std::map<MaterialProperty, bool> bools;

public:
	MaterialProperty getProperties() const
	{
		MaterialProperty properties = static_cast<MaterialProperty>(0);
		for (const auto& pair : textures) properties |= pair.first;
		for (const auto& pair : colors) properties |= pair.first;
		for (const auto& pair : floats) properties |= pair.first;
		for (const auto& pair : bools) properties |= pair.first;
		return properties;
	}

	float getFloat(MaterialProperty prop, float def = 0)
	{
		auto temp = floats.find(prop);
		return temp == floats.end() ? def : temp->second;
	}

	std::shared_ptr<Texture2D> getTexture(MaterialProperty prop)
	{
		auto temp = textures.find(prop);
		return temp == textures.end() ? nullptr : temp->second;
	}

	const Color& getColor(MaterialProperty prop, const Color& def)
	{
		auto temp = colors.find(prop);
		return temp == colors.end() ? def : temp->second;
	}

	void setFloat(MaterialProperty prop, float value)
	{
		floats[prop] = value;
	}

	void setBool(MaterialProperty prop, bool value)
	{
		bools[prop] = value;
	}

	void setColor(MaterialProperty prop, const Color& value)
	{
		colors[prop] = value;
	}

	void setTexture(MaterialProperty prop, const std::shared_ptr<Texture2D> value)
	{
		textures[prop] = value;
	}

	bool hasTexture(MaterialProperty prop)
	{
		return textures.find(prop) != textures.end();
	}
};

struct MeshRenderer : Component
{
	std::vector<Transform*> bones = {};
	bool castShadows = false;

	std::shared_ptr<Material> material;

	MaterialProperty getProperties() const
	{
		return material->getProperties();
	}
	
	float getFloat(MaterialProperty prop, float def = 0) const
	{
		return material->getFloat(prop, def);
	}

	std::shared_ptr<Texture2D> getTexture(MaterialProperty prop) const
	{
		return material->getTexture(prop);
	}

	const Color& getColor(MaterialProperty prop, const Color& def) const
	{
		return material->getColor(prop, def);
	}

	void setFloat(MaterialProperty prop, float value)
	{
		material->setFloat(prop, value);
	}

	void setBool(MaterialProperty prop, bool value)
	{
		material->setBool(prop, value);
	}

	void setColor(MaterialProperty prop, const Color& value)
	{
		material->setColor(prop, value);
	}

	void setTexture(MaterialProperty prop, const std::shared_ptr<Texture2D> value)
	{
		material->setTexture(prop, value);
	}

	bool hasTexture(MaterialProperty prop) const
	{
		return material->hasTexture(prop);
	}
};
