#pragma once

#include <GL/glew.h>
#include <functional>
#include <array>

template<typename T> struct GLType { operator GLenum() const { return 0; } };
template<> struct GLType<float> { operator GLenum() const { return GL_FLOAT; } };
template<> struct GLType<unsigned char> { operator GLenum() const { return GL_UNSIGNED_BYTE; } };

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
enum Filter : GLint
{
	LINEAR = GL_LINEAR,
	NEAREST = GL_NEAREST
};
enum Wrap : GLint
{
	REPEAT = GL_REPEAT,
	CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE
};

template<GLenum target, GLenum binding>
class Texture
{
	static std::array<GLuint, 32> boundTextures;
	static GLuint activeUnit;
protected:
	GLuint texture;
public:
	Texture() {
		glGenTextures(1, &texture);
	}
	~Texture() {
		glDeleteTextures(1, &texture);
	}
	void bind(GLenum unit) const {
		if(activeUnit != unit){
			glActiveTexture(GL_TEXTURE0 + (activeUnit = unit));
		}
		if(boundTextures[activeUnit] != this->texture){
			glBindTexture(target, boundTextures[activeUnit] = this->texture);
		}
	}
	void unBind(GLenum unit) const {
		if(activeUnit != unit){
			glActiveTexture(GL_TEXTURE0 + (activeUnit = unit));
		}
		if(boundTextures[activeUnit] == this->texture){
			glBindTexture(target, boundTextures[activeUnit] = 0);
		}
	}
	void setFilter(Filter min, Filter mag) {
		temporaryBind([min, mag]() {
			glTexParameteri(target, GL_TEXTURE_MIN_FILTER, min);
			glTexParameteri(target, GL_TEXTURE_MAG_FILTER, mag);
		});
	}
protected:
	void temporaryBind(std::function<void ()> bindOperation) {
		GLuint boundTexture = boundTextures[activeUnit];

		if (boundTexture == this->texture) {
			bindOperation();
		} else {
			glBindTexture(target, boundTextures[activeUnit] = this->texture);
			bindOperation();
			glBindTexture(target, boundTextures[activeUnit] = boundTexture);
		}
	}
};

template<GLenum target, GLenum binding>
std::array<GLuint, 32> Texture<target, binding>::boundTextures;

template<GLenum target, GLenum binding>
GLuint Texture<target, binding>::activeUnit = 0;
