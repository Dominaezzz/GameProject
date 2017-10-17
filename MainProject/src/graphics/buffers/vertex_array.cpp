#include "vertex_array.h"
#include <utility>
#include <algorithm>

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &vao);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &vao);
}

void VertexArray::bind() const
{
	glBindVertexArray(vao);
}

void VertexArray::unBind() const
{
	glBindVertexArray(0);
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

size_t VertexArray::getAttributeCount() const
{
	return vertexAttributes.size();
}

size_t VertexArray::getBufferCount() const
{
	return vertexBuffers.size();
}

std::shared_ptr<VertexBuffer> VertexArray::operator[](const int index) const
{
	auto temp = std::find_if(vertexAttributes.begin(), vertexAttributes.end(),
		[index](const VertexAttribute& attrib) -> bool {
			return attrib.index == index;
		}
	);
	return temp == vertexAttributes.end() ? nullptr : vertexBuffers[temp->bufferIndex];
}
