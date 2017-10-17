#pragma once

#include <iostream>
#include <string>
#include <memory>
#include "texture.h"

class Texture2D : public Texture<GL_TEXTURE_2D, GL_TEXTURE_BINDING_2D>
{
private:
	GLsizei width;
	GLsizei height;
	PixelFormat internalFormat;
public:
	Texture2D(GLsizei width, GLsizei height, PixelFormat internalFormat);
	inline GLsizei getWidth() const;
	inline GLsizei getHeight() const;
	inline PixelFormat getFormat() const;
	void setWrap(const Wrap s, const Wrap t);

	template<typename Type>
	void setData(const void* pixels, PixelFormat format);
	template<typename Type>
	void setSubData(const void* pixels, PixelFormat format, int x, int y, int width, int height);

	static std::shared_ptr<Texture2D> fromFile(const std::string& path, bool useMipMaps = false);
};
