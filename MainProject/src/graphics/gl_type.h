#pragma once
#include <GL/glew.h>


template<typename T> struct GLType { operator GLenum() const { throw std::runtime_error("No type found."); } };
template<> struct GLType<float> { operator GLenum() const { return GL_FLOAT; } };
template<> struct GLType<unsigned char> { operator GLenum() const { return GL_UNSIGNED_BYTE; } };