#pragma once
#include <GL/glew.h>
#include <string>
#include "gl_resource.h"

enum class ShaderType : GLenum
{
	VertexShader = GL_VERTEX_SHADER,
	FragmentShader = GL_FRAGMENT_SHADER
};

class Shader final : GLResource
{
	GLuint shader;
public:
	explicit Shader(ShaderType type, const std::string& source, bool isPath = true);
	~Shader();
	std::string getInfoLog() const;
};
