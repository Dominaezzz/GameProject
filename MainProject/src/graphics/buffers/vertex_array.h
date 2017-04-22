#pragma once

#include <vector>
#include <utility>
#include <memory>
#include <algorithm>
#include "vertex_buffer.h"

struct VertexAttribute{
	GLuint index;
	std::shared_ptr<VertexBuffer> vertexBuffer;
};

enum VertexAttrib{
	Position = 0,
	TexCoords = 1,
	Normal = 2,
	Tangent = 3,
	BiTangent = 4,
	ColorDiffuse = 5,
	BoneWeight = 6,

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
private:
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
	template<typename T>
	void setAttribute(std::shared_ptr<VertexBuffer> buffer, unsigned int index, GLenum type, int stride, int offset, bool normalized = false){
		bind();
		buffer->bind();
		{
			// TODO 14/1/2017 implement normalized.
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index, sizeof(T) / sizeof(float), type, GL_FALSE/**(GLboolean) normalized**/, stride, (void *)(offset));

			VertexAttribute attribute;
			attribute.index = index;
			attribute.vertexBuffer = buffer;
			vertexAttributes.push_back(attribute);
		}
		buffer->unBind();
		unBind();
	}
};
