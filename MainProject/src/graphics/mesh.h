#pragma once

#include "shaderprogram.h"
#include "buffers/vertex_array.h"
#include "buffers/index_buffer.h"

class Mesh
{
	VertexArray vertexArray;
	IndexBuffer indexBuffer;
public:
	Mesh();
	~Mesh();
	void bind() const;
	void unBind() const;
	void render(GLenum mode, GLuint offset = 0, int count = -1, bool autoBind = true) const;
	void renderInstanced(GLenum mode, GLsizei instances, GLuint offset = 0, GLsizei count = -1, bool autoBind = true) const;
	void setIndices(const int* data, size_t size);
	size_t getBufferCount() const;
	template<typename T, typename Type = float>
	void setVertexAttribute(std::shared_ptr<VertexBuffer> buffer, unsigned int index, int stride, int offset, bool normalized = false, bool isInstanced = false)
	{
		vertexArray.setAttribute<T, Type>(buffer, index, stride, offset, normalized, isInstanced);
	}
};
