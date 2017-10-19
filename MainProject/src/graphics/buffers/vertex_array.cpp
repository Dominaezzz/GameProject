#include "vertex_array.h"
#include <utility>

VertexArray::VertexArray() : vertexBuffers(1)
{
	glCreateVertexArrays(1, &vao);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &vao);
}

GLuint VertexArray::getId() const
{
	return this->vao;
}

void VertexArray::bind() const
{
	glBindVertexArray(vao);
}

void VertexArray::unBind() const
{
	glBindVertexArray(0);
}

void VertexArray::setVertexBuffer(const size_t index, const std::shared_ptr<VertexBuffer>& vertexBuffer, const size_t stride, const size_t offset)
{
	if (vertexBuffers.size() <= index) vertexBuffers.resize(index + 1);
	vertexBuffers[index] = std::make_pair(vertexBuffer, stride);
	glVertexArrayVertexBuffer(this->vao, index, vertexBuffer->getId(), offset, stride);
}

void VertexArray::setIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
{
	this->indexBuffer = indexBuffer;
#ifdef USE_OPENGL_DSA
	glVertexArrayElementBuffer(this->vao, indexBuffer->getId());
#else
	bind();
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer->getId());
	}
	unBind();
#endif
}

void VertexArray::draw(PrimitiveType type, size_t offset, size_t count, bool autoBind) const
{
	if (autoBind)bind();
	{
		if (indexBuffer == nullptr)
		{
			glDrawArrays(static_cast<GLenum>(type), offset, count);
		}
		else
		{
			glDrawElements(static_cast<GLenum>(type), count, GL_UNSIGNED_INT, reinterpret_cast<void*>(offset * sizeof(int)));
		}
	}
	if (autoBind)unBind();
}

void VertexArray::drawInstanced(PrimitiveType type, size_t offset, size_t count, size_t instances, bool autoBind) const
{
	if (autoBind)bind();
	{
		if (indexBuffer == nullptr)
		{
			glDrawArraysInstanced(static_cast<GLenum>(type), offset, count, instances);
		}
		else
		{
			glDrawElementsInstanced(static_cast<GLenum>(type), count, GL_UNSIGNED_INT, reinterpret_cast<void*>(offset), instances);
		}
	}
	if (autoBind)unBind();
}

size_t VertexArray::getBufferCount() const
{
	return vertexBuffers.size();
}

std::shared_ptr<VertexBuffer> VertexArray::operator[](const int index) const
{
	return vertexBuffers[index].first;
}
