#pragma once
#include <GL/glew.h>

class GLResource
{
public:
	GLResource() = default;
	GLResource(const GLResource&) = delete;
	GLResource& operator=(const GLResource&) = delete;
	virtual ~GLResource() = default;
	virtual GLuint getId() const = 0;
};
