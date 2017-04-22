#include "vertex_array.h"

VertexArray::VertexArray() {
	glGenVertexArrays(1, &vao);
}

VertexArray::~VertexArray() {
	glDeleteVertexArrays(1, &vao);
}

void VertexArray::bind() const {
	glBindVertexArray(vao);
}

void VertexArray::unBind() const {
	glBindVertexArray(0);
}

size_t VertexArray::getAttributeCount() const {
	return vertexAttributes.size();
}

size_t VertexArray::getBufferCount() const {
	size_t count = 0;
	for (size_t i = 0; i < vertexAttributes.size(); ++i) {
		bool found = false;
		for (size_t j = 0; j < vertexAttributes.size() - i - 1; ++j) {
			if(vertexAttributes[i].vertexBuffer == vertexAttributes[j].vertexBuffer)
			{
				found = true;
				break;
			}
		}
		if(!found)count++;
	}
	return count;
}

std::shared_ptr<VertexBuffer> VertexArray::operator[](const int index) const {
	auto temp = std::find_if(
			vertexAttributes.begin(),
			vertexAttributes.end(),
			[index](const VertexAttribute& attrib) -> bool {
				return attrib.index == index;
			}
	);
	return temp == vertexAttributes.end() ? nullptr : temp->vertexBuffer;
}
