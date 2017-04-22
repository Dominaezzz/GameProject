#include "index_buffer.h"

IndexBuffer::IndexBuffer() : count(0)
{
}

void IndexBuffer::setData(const int* data, unsigned int size) {
	Buffer::setData(data, size, StaticDraw);
	this->count = size / sizeof(int);
}
