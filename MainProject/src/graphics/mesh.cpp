#include "mesh.h"

Mesh::Mesh() {}

Mesh::~Mesh() {}

void Mesh::bind() {
	vertexArray.bind();
	if (indexBuffer.getCount() > 0) {
		indexBuffer.bind();
	}
}

void Mesh::unBind() {
	vertexArray.unBind();
	if (indexBuffer.getCount() > 0) {
		indexBuffer.unBind();
	}
}

void Mesh::render(GLenum mode, GLuint offset, int count, bool autoBind) {
	if (count == 0 || vertexArray.getBufferCount() == 0)return;

	if (autoBind)bind();
	{
		if (indexBuffer.getCount() > 0)
		{
			glDrawElements(mode, count < 0 ? indexBuffer.getCount() : count, GL_UNSIGNED_INT, (void*)(offset * sizeof(int)));
		}
		else
		{
			glDrawArrays(mode, offset, count < 0 ? vertexArray[0]->getSize() : count);
		}
	}
	if (autoBind)unBind();
}

void Mesh::renderInstanced(GLenum mode, GLsizei instances, GLuint offset, GLsizei count, bool autoBind) {
	if (count == 0 || vertexArray.getBufferCount() == 0)return;

	if (autoBind)bind();
	{
		if (indexBuffer.getCount() > 0)
		{
			glDrawElementsInstanced(mode, count < 0 ? indexBuffer.getCount() : count, GL_UNSIGNED_INT, (void*)(offset * sizeof(int)), instances);
		}
		else
		{
			glDrawArraysInstanced(mode, offset, count < 0 ? vertexArray[0]->getSize() : count, instances);
		}
	}
	if (autoBind)unBind();
}
