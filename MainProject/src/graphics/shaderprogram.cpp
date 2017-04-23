#include "shaderprogram.h"


ShaderProgram::ShaderProgram()
{
	program = glCreateProgram();
}

ShaderProgram::~ShaderProgram()
{
	for (auto shader : shaders) glDetachShader(program, shader->getId());
	glDeleteProgram(program);
}

void ShaderProgram::begin() const
{
	glUseProgram(program);
}

void ShaderProgram::end() const
{
	glUseProgram(0);
}

std::string ShaderProgram::getInfoLog() const
{
	GLint logLength;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);

	std::string log(size_t(logLength), ' ');
	glGetProgramInfoLog(program, logLength, &logLength, &log[0]);

	return log;
}

void ShaderProgram::compile()
{
	GLint success = 0;

	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (success == GL_FALSE) throw std::runtime_error(getInfoLog());

	glValidateProgram(program);
	glGetProgramiv(program, GL_VALIDATE_STATUS, &success);
	if (success == GL_FALSE) throw std::runtime_error(getInfoLog());

	GLchar temp[128];
	GLsizei length;
	GLenum type;
	GLint size;

	GLint count;
	GLint maxNameLength;

	glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &count);
	glGetProgramiv(program, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxNameLength);
	for (GLuint i = 0; i < count; i++)
	{
		glGetActiveAttrib(program, i, sizeof(temp), &length, &size, &type, temp);

		std::string name(temp, length);
		attributes[name] = glGetAttribLocation(program, name.c_str());
	}

	glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &count);
	glGetProgramiv(program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxNameLength);
	for (GLuint i = 0; i < count; i++)
	{
		glGetActiveUniform(program, i, sizeof(temp), &length, &size, &type, temp);

		std::string name(temp, length);
		uniforms[name] = glGetUniformLocation(program, name.c_str());
	}
}

void ShaderProgram::attach(std::shared_ptr<Shader> shader)
{
	glAttachShader(program, shader->getId());
	shaders.push_back(shader);
}

void ShaderProgram::add(ShaderType type, const std::string & source, bool isPath)
{
	attach(std::make_shared<Shader>(type, source, isPath));
}

GLuint ShaderProgram::getAttributeLocation(const std::string& name) {
	auto pair = attributes.find(name);
	if (pair == attributes.end())
	{
		GLuint location = (GLuint) glGetAttribLocation(program, name.c_str());
		attributes[name] = location;
		return location;
	}
	return pair->second;
}

GLuint ShaderProgram::getUniformLocation(const std::string& name) {
	auto pair = uniforms.find(name);
	if (pair == uniforms.end()) {
		GLuint location = (GLuint) glGetUniformLocation(program, name.c_str());
		uniforms[name] = location;
		return location;
	}
	return pair->second;
}
