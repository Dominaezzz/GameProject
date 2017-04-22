#pragma once

#include "buffer.h"

class IndexBuffer final : public Buffer<GL_ELEMENT_ARRAY_BUFFER>
{
	unsigned int count;
public:
	IndexBuffer();
	unsigned int getCount() const { return count; }
	void setData(const int* data, unsigned int size);
};
