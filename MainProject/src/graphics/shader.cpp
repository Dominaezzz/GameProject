#include "shader.h"
#include <fstream>

Shader::Shader(ShaderType type, const std::string & source, bool isPath)
{
	shader = glCreateShader(GLenum(type));

	if (isPath)
	{
		std::ifstream file(source);
		std::string temp;
		std::string file_contents;
		while (std::getline(file, temp))
		{
			file_contents += temp;
			file_contents.push_back('\n');
		}

		const GLchar* text = file_contents.c_str();
		glShaderSource(shader, 1, &text, nullptr);
	}
	else
	{
		const GLchar* text = source.c_str();
		glShaderSource(shader, 1, &text, nullptr);
	}

	glCompileShader(shader);

	GLint success = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (success == GL_FALSE) throw std::runtime_error(getInfoLog());
}

Shader::~Shader()
{
	glDeleteShader(shader);
}

GLuint Shader::getId() const
{
	return shader;
}

std::string Shader::getInfoLog() const
{
	GLint logLength;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);

	std::string infoLog(size_t(logLength), ' ');

	glGetShaderInfoLog(shader, logLength, &logLength, &infoLog[0]);

	return infoLog;
}
