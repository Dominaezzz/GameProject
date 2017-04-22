#include "texture2d.h"
//#define STB_IMAGE_IMPLEMENTATION
#include "../../utils/stb_image.h"


Texture2D::Texture2D(GLsizei width, GLsizei height, PixelFormat internalFormat) {
	this->width = width;
	this->height = height;
	this->internalFormat = internalFormat;

	setFilter(LINEAR, LINEAR);
	setWrap(REPEAT, REPEAT);

	temporaryBind([this]() {
		glTexImage2D(GL_TEXTURE_2D, 0, this->internalFormat, this->width, this->height, 0, this->internalFormat, GL_UNSIGNED_BYTE, nullptr);

		//  if (useMipMaps)
		//  {
		//  	glGenerateMipmap(GL_TEXTURE_2D);
		//  }
	});
}

Texture2D::Texture2D(std::string path, bool useMipMaps) {
	internalFormat = RGBA;
	int components;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &components, STBI_default);

	PixelFormat format;
	switch (components)
	{
		case STBI_grey:
			format = LUMINANCE;
			break;
		case STBI_grey_alpha:
			format = LUMINANCE_ALPHA;
			break;
		case STBI_rgb:
			format = RGB;
			break;
		case STBI_rgb_alpha:
			format = RGBA;
			break;
		default:
			throw std::runtime_error("None matching components: " + std::to_string(components));
	}

	temporaryBind([this, data, format, useMipMaps]() {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
//		glTexStorage2D(GL_TEXTURE_2D, 0, internalFormat, width, height);
		if (useMipMaps)glGenerateMipmap(GL_TEXTURE_2D);
	});

	stbi_image_free(data);
}

template<typename Type>
void Texture2D::setData(const void* pixels, PixelFormat format, int width, int height) {
	temporaryBind([this, pixels, format, width, height](){
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GLType<Type>(), pixels);
		this->width = width;
		this->height = height;
	});
}

template<typename Type>
void Texture2D::setData(const void* pixels, PixelFormat format) {
	setSubData<Type>(pixels, format, 0, 0, width, height);
}

template<typename Type>
void Texture2D::setSubData(const void* pixels, PixelFormat format, int x, int y, int width, int height) {
	temporaryBind([this, pixels, format, x, y, width, height]() {
		glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, width, height, format, GLType<Type>(), pixels);
	});
}

std::shared_ptr<Texture2D> Texture2D::fromFile(std::string path, bool useMipMaps) {
	stbi_set_flip_vertically_on_load(true);

	int width, height, components;
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &components, STBI_default);

	PixelFormat format;
	switch (components)
	{
		case STBI_grey:
			format = LUMINANCE;
			break;
		case STBI_grey_alpha:
			format = LUMINANCE_ALPHA;
			break;
		case STBI_rgb:
			format = RGB;
			break;
		case STBI_rgb_alpha:
			format = RGBA;
			break;
		default:
			throw std::runtime_error("None matching components: " + std::to_string(components));
	}

	auto texture = std::make_shared<Texture2D>(width, height, RGBA);
	texture->setData<GLubyte>(data, format);

	stbi_image_free(data);

	return texture;
}
