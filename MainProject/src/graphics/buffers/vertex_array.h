#pragma once

#include <vector>
#include <utility>
#include <memory>
#include <algorithm>
#include "vertex_buffer.h"
#include "../gl_type.h"


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
		std::shared_ptr<VertexBuffer> vertexBuffer;
	};
	GLuint vao;
	std::vector<VertexAttribute> vertexAttributes;
public:
	VertexArray();
	~VertexArray();
	void bind() const;
	void unBind() const;
	size_t getAttributeCount() const ;
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
			glVertexAttribPointer(index, sizeof(T) / sizeof(float), GLType<Type>(), GL_FALSE, stride, (void *)(offset));
//			glVertexAttribPointer(index, sizeof(T) / sizeof(float), GLType<Type>(), normalized ? GL_TRUE : GL_FALSE, stride, (void *)(offset));
			if (isInstanced) glVertexAttribDivisor(index, 1);

			VertexAttribute attribute;
			attribute.index = index;
			attribute.vertexBuffer = buffer;
			vertexAttributes.push_back(attribute);
		}
		buffer->unBind();
		unBind();
	}
};
