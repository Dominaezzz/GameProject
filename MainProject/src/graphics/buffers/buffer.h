#pragma once

#include <GL/glew.h>
#include "../gl_resource.h"

enum BufferUsage : GLenum
{
	StaticDraw = GL_STATIC_DRAW,
	StaticRead = GL_STATIC_READ,
	StaticCopy = GL_STATIC_COPY,

	DynamicDraw = GL_DYNAMIC_DRAW,
	DynamicRead = GL_DYNAMIC_READ,
	DynamicCopy = GL_DYNAMIC_COPY,

	StreamDraw = GL_STREAM_DRAW,
	StreamRead = GL_STREAM_READ,
	StreamCopy = GL_STREAM_COPY
};

template<GLenum target>
class Buffer : public virtual GLResource
{
	static GLuint boundBuffer;
	GLuint buffer;
	GLuint size;
public:
	explicit Buffer(const size_t size = 0, const BufferUsage usage = StaticDraw) : Buffer(nullptr, size, usage)
	{
	}
	Buffer(const void* data, const size_t size, const BufferUsage usage = StaticDraw) : size(size)
	{
#ifdef USE_OPENGL_DSA
		glCreateBuffers(1, &buffer);
#else
		glGenBuffers(1, &buffer);
#endif
		setData(data, size, usage);
	}
	void bind() const
	{
		if (buffer != boundBuffer) glBindBuffer(target, boundBuffer = buffer);
	}
	void unBind() const
	{
		if (buffer == boundBuffer) glBindBuffer(target, boundBuffer = 0);
	}
	GLuint getId() const override
	{
		return buffer;
	}
	GLuint getSize() const
	{
		return size;
	}
protected:
	~Buffer()
	{
		unBind();
		glDeleteBuffers(1, &buffer);
	}

	void setData(const void* data, const size_t size, const BufferUsage usage)
	{
#ifdef USE_OPENGL_DSA
		glNamedBufferData(this->buffer, size, data, usage);
#else
		bind();
		glBufferData(target, size, data, usage);
#endif
		this->size = size;
	}
	void setSubData(const void* data, const size_t offset, const size_t size)
	{
		if ((offset + size) > this->size) throw std::invalid_argument("");
#ifdef USE_OPENGL_DSA
		glNamedBufferSubData(this->buffer, offset, size, data);
#else
		bind();
		glBufferSubData(target, offset, size, data);
#endif
	}
	void getSubData(void* data, const size_t offset, const size_t size)
	{
		if ((offset + size) > this->size) throw std::invalid_argument("");
#ifdef USE_OPENGL_DSA
		glGetNamedBufferSubData(this->buffer, offset, size, data);
#else
		bind();
		glGetBufferSubData(target, offset, size, data);
#endif
	}
};

template<GLenum target>
GLuint Buffer<target>::boundBuffer = 0;

template<GLuint rangeTarget>
class BufferRange : public virtual GLResource
{
//	static std::map<GLuint, Ranges?> boundBuffers;
protected:
	~BufferRange() = default;
public:
	void bindBase(const int index) const
	{
		glBindBufferBase(rangeTarget, index, getId());
	}
	void unBindBase(const int index) const
	{
		glBindBufferBase(rangeTarget, index, 0);
	}
	void bindRange(const int index, const int offset, const int size) const
	{
		glBindBufferRange(rangeTarget, index, getId(), offset, size);
	}
	void unBindRange(const int index, const int offset, const int size) const
	{
		glBindBufferRange(rangeTarget, index, 0, offset, size);
	}
};
