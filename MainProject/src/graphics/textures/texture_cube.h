#pragma once

#include <iostream>
#include <string>
#include <memory>
#include "texture.h"

enum CubeMapFace : GLint
{
	Right = GL_TEXTURE_CUBE_MAP_POSITIVE_X,
	Left = GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
	Top = GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
	Bottom = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
	Back = GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
	Front = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
};

class TextureCube : public Texture<GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BINDING_CUBE_MAP>
{
	GLsizei size;
	PixelFormat internalFormat;
public:
	TextureCube(GLsizei size, PixelFormat internalFormat);
	inline GLsizei getSize() const;
	inline PixelFormat getFormat() const;
	void setWrap(Wrap s, Wrap t, Wrap r);

	template<typename Type>
	void setData(CubeMapFace face, void* pixels, PixelFormat format, int size);
	template<typename Type>
	void setData(CubeMapFace face, const void* pixels, PixelFormat format);
	template<typename Type>
	void setSubData(CubeMapFace face, const void* pixels, PixelFormat format, int x, int y, int width, int height);

	static std::unique_ptr<TextureCube> fromFile(std::string right, std::string left, std::string top, std::string bottom, std::string back, std::string front);
};
