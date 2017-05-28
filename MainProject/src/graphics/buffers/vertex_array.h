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

class VertexArray
{
	struct VertexAttribute
	{
		GLuint index;
		size_t bufferIndex;
	};
	GLuint vao;
	std::vector<VertexAttribute> vertexAttributes;
	std::vector<std::shared_ptr<VertexBuffer>> vertexBuffers;
	std::shared_ptr<IndexBuffer> indexBuffer;
public:
	VertexArray();
	~VertexArray();
	void bind() const;
	void unBind() const;
	void setIndexBuffer(std::shared_ptr<IndexBuffer> indexBuffer);
	void draw(PrimitiveType type, size_t offset, size_t count, bool autoBind = true) const;
	void drawInstanced(PrimitiveType type, size_t offest, size_t count, size_t instances, bool autoBind = true) const;
	size_t getAttributeCount() const;
	size_t getBufferCount() const;
	std::shared_ptr<VertexBuffer> operator[](const int index) const;
	template<typename T, typename Type = float>
	void setAttribute(std::shared_ptr<VertexBuffer> buffer, unsigned int index, int stride, int offset, bool normalized = false, bool isInstanced = false)
	{
		bind();
		buffer->bind();
		{
			// TODO 14/1/2017 implement normalized.
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index, sizeof(T) / sizeof(Type), GLType<Type>(), GL_FALSE, stride, reinterpret_cast<void *>(offset));
			if (isInstanced) glVertexAttribDivisor(index, 1);

			VertexAttribute attribute;
			attribute.index = index;
			auto it = std::find(vertexBuffers.begin(), vertexBuffers.end(), buffer);
			if(it == vertexBuffers.end())
			{
				attribute.bufferIndex = vertexBuffers.size();
				vertexBuffers.push_back(buffer);
			}
			else
			{
				attribute.bufferIndex = std::distance(vertexBuffers.begin(), it);
			}
			vertexAttributes.push_back(attribute);
		}
		buffer->unBind();
		unBind();
	}
};
