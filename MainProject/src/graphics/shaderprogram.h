#pragma once

#include <GL/glew.h>
#include <unordered_map>
#include <vector>
#include <string>

class ShaderProgram
{
	GLuint program;
	std::vector<GLuint> shaders;
	std::unordered_map<std::string, GLuint> attributes;
	std::unordered_map<std::string, GLuint> uniforms;

	void addShader(GLenum type, const std::string& source, bool isPath);
public:
	ShaderProgram();
	~ShaderProgram();
	void begin() const;
	void end() const;
	void compile();
	void addVertexShader(const std::string& source, bool isPath = true);
	void addFragmentShader(const std::string& source, bool isPath = true);
	GLuint getAttributeLocation(const std::string& name);
	GLuint getUniformLocation(const std::string& name);
};
