#include "mesh.h"

Mesh::Mesh() {}

Mesh::~Mesh() {}

void Mesh::render(bool autoBind) const
{
	VertexArray::draw(primitiveType, offset, count, autoBind);
}

void Mesh::renderInstanced(GLsizei instances, bool autoBind) const
{
	VertexArray::drawInstanced(primitiveType, offset, count, instances, autoBind);
}

void Mesh::setIndices(const int* data, size_t size)
{
	if (indexBuffer == nullptr) VertexArray::setIndexBuffer(indexBuffer = std::make_shared<IndexBuffer>());
	indexBuffer->setData(data, size);
}

size_t Mesh::getBufferCount() const
{
	return VertexArray::getBufferCount();
}
