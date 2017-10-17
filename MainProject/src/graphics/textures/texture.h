#pragma once

#include <GL/glew.h>
#include <functional>
#include <array>
#include "../gl_type.h"
#include "../gl_resource.h"

enum PixelFormat : GLenum
{
	LUMINANCE = GL_LUMINANCE,
	LUMINANCE_ALPHA = GL_LUMINANCE_ALPHA,
	RED = GL_RED,
	RG = GL_RG,
	RGB = GL_RGB,
	RGBA = GL_RGBA,
	DEPTH_COMPONENT = GL_DEPTH_COMPONENT,
	STENCIL_INDEX = GL_STENCIL_INDEX,
	DEPTH_STENCIL = GL_DEPTH_STENCIL
};
enum class MinFilter : GLint
{
	NEAREST = GL_NEAREST,
	LINEAR = GL_LINEAR,
	NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
	LINEAR_MIPMAP_NEAREST = GL_LINEAR_MIPMAP_NEAREST,
	NEAREST_MIPMAP_LINEAR = GL_NEAREST_MIPMAP_LINEAR,
	LINEAR_MIPMAP_LINEAR = GL_LINEAR_MIPMAP_LINEAR
};
enum class MagFilter : GLint
{
	NEAREST = GL_NEAREST,
	LINEAR = GL_LINEAR
};
enum Wrap : GLint
{
	REPEAT = GL_REPEAT,
	CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE
};

class Hack
{
#ifndef USE_OPENGL_DSA
protected:
	static GLuint activeUnit;
#endif
};

template<GLenum target, GLenum binding>
class Texture : GLResource, Hack
{
	static std::array<GLuint, 32> boundTextures;
protected:
	GLuint texture;
public:
	Texture() {
#ifdef USE_OPENGL_DSA
		glCreateTextures(target, 1, &texture);
#else
		glGenTextures(1, &texture);
#endif
	}
	~Texture() {
		glDeleteTextures(1, &texture);
	}
	GLuint getId() const override { return texture; }
	void bind(const GLenum unit) const {
#ifdef USE_OPENGL_DSA
		glBindTextureUnit(unit, boundTextures[unit] = this->texture);
#else
		if(activeUnit != unit){
			glActiveTexture(GL_TEXTURE0 + (activeUnit = unit));
		}
		if(boundTextures[activeUnit] != this->texture){
			glBindTexture(target, boundTextures[activeUnit] = this->texture);
		}
#endif
	}
	void unBind(const GLenum unit) const {
#ifdef USE_OPENGL_DSA
		if (boundTextures[unit] == this->texture) {
			glBindTextureUnit(unit, boundTextures[unit] = 0);
		}
#else
		if(activeUnit != unit){
			glActiveTexture(GL_TEXTURE0 + (activeUnit = unit));
		}
		if(boundTextures[activeUnit] == this->texture){
			glBindTexture(target, boundTextures[activeUnit] = 0);
		}
#endif
	}
	void setFilter(MinFilter min, MagFilter mag) {
#ifdef USE_OPENGL_DSA
		glTextureParameteri(this->texture, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(min));
		glTextureParameteri(this->texture, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(mag));
#else
		temporaryBind([min, mag]() {
			glTexParameteri(target, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(min));
			glTexParameteri(target, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(mag));
		});
#endif
	}
	void generateMipmap()
	{
#ifdef USE_OPENGL_DSA
		glGenerateTextureMipmap(this->texture);
#else
		temporaryBind([]()
		{
			glGenerateMipmap(target);
		});
#endif
	}
#ifndef USE_OPENGL_DSA
protected:
	void temporaryBind(const std::function<void ()>& bindOperation) {
		GLuint boundTexture = boundTextures[activeUnit];

		if (boundTexture == this->texture) {
			bindOperation();
		} else {
			glBindTexture(target, boundTextures[activeUnit] = this->texture);
			bindOperation();
			glBindTexture(target, boundTextures[activeUnit] = boundTexture);
		}
	}
#endif
};

template<GLenum target, GLenum binding>
std::array<GLuint, 32> Texture<target, binding>::boundTextures;
