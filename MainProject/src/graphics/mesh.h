#pragma once

#include "shaderprogram.h"
#include "buffers/vertex_array.h"
#include "buffers/index_buffer.h"

class Mesh : VertexArray
{
	std::shared_ptr<IndexBuffer> indexBuffer;
public:
	PrimitiveType primitiveType = PrimitiveType::Triangles;
	size_t offset = 0;
	size_t count = 0;

	Mesh();
	~Mesh();
	using VertexArray::bind;
	using VertexArray::unBind;
	using VertexArray::getBufferCount;
	void render(bool autoBind = true) const;
	void renderInstanced(GLsizei instances, bool autoBind = true) const;
	void setIndices(const int* data, size_t size);
	using VertexArray::setVertexBuffer;
	using VertexArray::setAttribute;
};
