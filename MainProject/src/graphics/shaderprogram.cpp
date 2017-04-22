#include "shaderprogram.h"
#include <fstream>
#include <iostream>


ShaderProgram::ShaderProgram() {
	program = glCreateProgram();
}

ShaderProgram::~ShaderProgram() {
	for (auto shader : shaders) {
		glDetachShader(program, shader);
		glDeleteShader(shader);
	}

	glDeleteProgram(program);
}

void ShaderProgram::begin() const {
	glUseProgram(program);
}

void ShaderProgram::end() const {
	glUseProgram(0);
}

void checkShaderError(GLuint shader, GLuint flag, bool isProgram) {
	GLint success = 0;

	if (isProgram) {
		glGetProgramiv(shader, flag, &success);
	} else {
		glGetShaderiv(shader, flag, &success);
	}

	if (success == GL_FALSE) {
		GLint logLength;

		if (isProgram) {
			glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &logLength);
		} else {
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
		}

		std::string log((unsigned int) logLength, ' ');

		if (isProgram) {
			glGetProgramInfoLog(shader, logLength, &logLength, &log[0]);
		} else {
			glGetShaderInfoLog(shader, logLength, &logLength, &log[0]);
		}

		throw std::runtime_error(log);
	}
}

void ShaderProgram::addShader(GLenum type, const std::string& source, bool isPath) {
	GLuint shader = glCreateShader(type);

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
		glShaderSource(shader, 1, &text, NULL);
	}
	else
	{
		const GLchar* text = source.c_str();
		glShaderSource(shader, 1, &text, NULL);
	}

	glCompileShader(shader);

	checkShaderError(shader, GL_COMPILE_STATUS, false);

	glAttachShader(program, shader);
	shaders.push_back(shader);
}

void ShaderProgram::addVertexShader(const std::string& source, bool isPath) {
	addShader(GL_VERTEX_SHADER, source, isPath);
}

void ShaderProgram::addFragmentShader(const std::string& source, bool isPath) {
	addShader(GL_FRAGMENT_SHADER, source, isPath);
}

void ShaderProgram::compile() {
	glLinkProgram(program);
	checkShaderError(program, GL_LINK_STATUS, true);

	glValidateProgram(program);
	checkShaderError(program, GL_VALIDATE_STATUS, true);

	GLchar temp[128];
	GLsizei length;
	GLenum type;
	GLint size;

	GLint count;
	GLint maxNameLength;

	glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &count);
	glGetProgramiv(program, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxNameLength);
	for (GLint i = 0; i < count; i++)
	{
		glGetActiveAttrib(program, i, sizeof(temp), &length, &size, &type, temp);

		std::string name(temp, length);
		attributes[name] = glGetAttribLocation(program, name.c_str());
	}

	glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &count);
	glGetProgramiv(program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxNameLength);
	for (GLint i = 0; i < count; i++)
	{
		glGetActiveUniform(program, i, sizeof(temp), &length, &size, &type, temp);

		std::string name(temp, length);
		uniforms[name] = glGetUniformLocation(program, name.c_str());
	}
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
