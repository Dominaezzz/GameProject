#pragma once
#include <GL/glew.h>

class GLResource
{
public:
	virtual ~GLResource() = default;
	virtual GLuint getId() const = 0;
};
