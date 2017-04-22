#pragma once

#include "buffer.h"

class VertexBuffer final : public Buffer<GL_ARRAY_BUFFER> {
	friend class VertexArray;
public:
	using Buffer::Buffer;
	using Buffer::setData;
	using Buffer::setSubData;
	using Buffer::getSubData;
};
