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
	inline GLsizei getWidth() const { return width; }
	inline GLsizei getHeight() const { return height; }
	inline PixelFormat getFormat() const { return internalFormat; }
	void setWrap(Wrap s, Wrap t) {
		temporaryBind([s, t]() {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, s);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, t);
		});
	}

	template<typename Type>
	void setData(const void* pixels, PixelFormat format, int width, int height);
	template<typename Type>
	void setData(const void* pixels, PixelFormat format);
	template<typename Type>
	void setSubData(const void* pixels, PixelFormat format, int x, int y, int width, int height);

	static std::shared_ptr<Texture2D> fromFile(std::string path, bool useMipMaps = false);
};
