#pragma once

#include <GL/glew.h>
#include <unordered_map>
#include <vector>
#include <string>
#include <memory>
#include "shader.h"

class ShaderProgram : public GLResource
{
	GLuint program;
	std::vector<std::shared_ptr<Shader>> shaders;
	std::unordered_map<std::string, GLuint> attributes;
	std::unordered_map<std::string, GLuint> uniforms;
public:
	ShaderProgram();
	~ShaderProgram();
	GLuint getId() const override;
	void begin() const;
	void end() const;
	void compile();
	void attach(std::shared_ptr<Shader> shader);
	void add(ShaderType type, const std::string& source, bool isPath = true);
	std::string getInfoLog() const;
	GLuint getAttributeLocation(const std::string& name);
	GLuint getUniformLocation(const std::string& name);
};
