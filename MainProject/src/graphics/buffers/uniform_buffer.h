#pragma once

#include "buffer.h"

class UniformBuffer final : public Buffer<GL_UNIFORM_BUFFER>, public BufferRange<GL_UNIFORM_BUFFER>
{
public:
	using Buffer::Buffer;
	using Buffer::setData;
	using Buffer::setSubData;
	using Buffer::getSubData;
};