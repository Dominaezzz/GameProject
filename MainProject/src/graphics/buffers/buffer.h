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
	Buffer(const void* data, const size_t size, const BufferUsage usage = StaticDraw)
	{
		glGenBuffers(1, &buffer);
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
		bind();
		glBufferData(target, size, data, usage);
		this->size = size;
	}
	void setSubData(const void* data, const size_t offset, const size_t size)
	{
		if ((offset + size) > this->size) throw std::invalid_argument("");
		bind();
		glBufferSubData(target, offset, size, data);
	}
	void getSubData(void* data, const size_t offset, const size_t size)
	{
		if (offset < 0 || offset >= this->size || (offset + size) > this->size) throw std::invalid_argument("");
		bind();
		glGetBufferSubData(target, offset, size, data);
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
	void bindBase(int index) const
	{
		glBindBufferBase(rangeTarget, index, getId());
	}
	void unBindBase(int index) const
	{
		glBindBufferBase(rangeTarget, index, 0);
	}
	void bindRange(int index, int offset, int size) const
	{
		glBindBufferRange(rangeTarget, index, getId(), offset, size);
	}
	void unBindRange(int index, int offset, int size) const
	{
		glBindBufferRange(rangeTarget, index, 0, offset, size);
	}
};
