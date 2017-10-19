#pragma once

#include <vector>
#include <memory>
#include "vertex_buffer.h"
#include "../gl_type.h"
#include "index_buffer.h"

enum class PrimitiveType : GLenum
{
	Points = GL_POINTS,
	Triangles = GL_TRIANGLES,
	TriangleStrip = GL_TRIANGLE_STRIP,
	TriangleFan = GL_TRIANGLE_FAN,
};

enum VertexAttrib
{
	Position = 0,
	TexCoords = 1,
	Normal = 2,
	Tangent = 3,
	BiTangent = 4,
	ColorDiffuse = 5,
//	BoneWeight = 6,
	Joints = 6,
	Weights = 7,

//	Ambient = 4,
//	Diffuse = 5,
//	Specular = 6,
//
//	TexCoords_DiffuseMap = 10,
//	TexCoords_NormalMap = 11,
//	TexCoords_SpecularMap = 12,
//	TexCoords_AmbientMap = 13,

	Max = 16
};

class VertexArray : public GLResource
{
#ifndef USE_OPENGL_DSA
	struct VertexAttribute
	{
		size_t size;
		GLenum type;
		bool normalize = false;
		size_t relativeOffset = 0;
	} vertexAttributes[16];
#endif
	GLuint vao;
	std::vector<std::pair<std::shared_ptr<VertexBuffer>, size_t>> vertexBuffers;
	std::shared_ptr<IndexBuffer> indexBuffer;
public:
	VertexArray();
	~VertexArray();
	GLuint getId() const override;
	void bind() const;
	void unBind() const;
	void setIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer);
	void setVertexBuffer(size_t index, const std::shared_ptr<VertexBuffer>& vertexBuffer, const size_t stride, const size_t offset = 0);
	template<typename T, typename Type = float>
	void setAttribute(const size_t bufferIndex, unsigned int index, int offset, bool normalized = false, bool isInstanced = false)
	{
#ifdef USE_OPENGL_DSA
		glEnableVertexArrayAttrib(this->vao, index);
		glVertexArrayAttribBinding(this->vao, index, bufferIndex);
		glVertexArrayAttribFormat(this->vao, index, sizeof(T) / sizeof(Type), GLType<Type>(), normalized, offset);
		if (isInstanced) glVertexArrayBindingDivisor(this->vao, bufferIndex, 1);
#else
		bind();
		const std::shared_ptr<VertexBuffer>& buffer = vertexBuffers[bufferIndex].first;
		buffer->bind();
		{
			// TODO 14/1/2017 implement normalized.
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index, sizeof(T) / sizeof(Type), GLType<Type>(), GL_FALSE, stride, reinterpret_cast<void *>(offset));
			if (isInstanced) glVertexAttribDivisor(index, 1);
		}
		buffer->unBind();
		unBind();

		VertexAttribute& attribute = vertexAttributes[index];
		attribute.size = sizeof(T) / sizeof(Type);
		attribute.type = GLType<Type>();
		attribute.normalize = false;
		attribute.relativeOffset = offset;
#endif
	}
	void draw(PrimitiveType type, size_t offset, size_t count, bool autoBind = true) const;
	void drawInstanced(PrimitiveType type, size_t offest, size_t count, size_t instances, bool autoBind = true) const;
	size_t getBufferCount() const;
	std::shared_ptr<VertexBuffer> operator[](const int index) const;
};
