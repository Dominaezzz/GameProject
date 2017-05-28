#pragma once

#include "shaderprogram.h"
#include "buffers/vertex_array.h"
#include "buffers/index_buffer.h"

class Mesh
{
	VertexArray vertexArray;
	std::shared_ptr<IndexBuffer> indexBuffer;
public:
	PrimitiveType primitiveType = PrimitiveType::Triangles;
	size_t offset = 0;
	size_t count = 0;

	Mesh();
	~Mesh();
	void bind() const;
	void unBind() const;
	void render(bool autoBind = true) const;
	void renderInstanced(GLsizei instances, bool autoBind = true) const;
	void setIndices(const int* data, size_t size);
	size_t getBufferCount() const;
	template<typename T, typename Type = float>
	void setVertexAttribute(std::shared_ptr<VertexBuffer> buffer, unsigned int index, int stride, int offset, bool normalized = false, bool isInstanced = false)
	{
		vertexArray.setAttribute<T, Type>(buffer, index, stride, offset, normalized, isInstanced);
	}
};
