#include "mesh.h"

Mesh::Mesh() {}

Mesh::~Mesh() {}

void Mesh::bind() const
{
	vertexArray.bind();
}

void Mesh::unBind() const
{
	vertexArray.unBind();
}

void Mesh::render(bool autoBind) const
{
	vertexArray.draw(primitiveType, offset, count, autoBind);
}

void Mesh::renderInstanced(GLsizei instances, bool autoBind) const
{
	vertexArray.drawInstanced(primitiveType, offset, count, instances, autoBind);
}

void Mesh::setIndices(const int* data, size_t size)
{
	if (indexBuffer == nullptr) vertexArray.setIndexBuffer(indexBuffer = std::make_shared<IndexBuffer>());
	indexBuffer->setData(data, size);
}

size_t Mesh::getBufferCount() const
{
	return vertexArray.getBufferCount();
}
