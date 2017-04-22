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
	template<typename T>
	void setVertexAttribute(std::shared_ptr<VertexBuffer> buffer, unsigned int index, GLenum type, int stride, int offset, bool normalized = false){
		vertexArray.setAttribute<T>(buffer, index, type, stride, offset, normalized);
	}
	inline void setIndices(const int* data, unsigned int size) { return indexBuffer.setData(data, size); }
	inline size_t getBufferCount() const { return vertexArray.getBufferCount(); }
	void bind();
	void unBind();
	void render(GLenum mode, GLuint offset = 0, int count = -1, bool autoBind = true);
	void renderInstanced(GLenum mode, GLsizei instances, GLuint offset = 0, GLsizei count = -1, bool autoBind = true);
};
