#pragma once
#include "../component.h"
#include <vector>
#include "tranform.h"

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

struct MeshRenderer : Component
{
	std::vector<Transform*> bones = {};
	bool castShadows = false;

	std::map<MaterialProperty, std::shared_ptr<Texture2D>> textures;
	std::map<MaterialProperty, Color> colors;
	std::map<MaterialProperty, float> floats;
	std::map<MaterialProperty, bool> bools;
	
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
