#include "texture_cube.h"
#include "../../utils/stb_image.h"

TextureCube::TextureCube(GLsizei size, PixelFormat internalFormat)
{
	this->size = size;
	this->internalFormat = internalFormat;

	temporaryBind([this]() {
		for (GLenum i = GL_TEXTURE_CUBE_MAP_POSITIVE_X; i <= GL_TEXTURE_CUBE_MAP_NEGATIVE_Z; ++i)
		{
			glTexImage2D(i, 0, this->internalFormat, this->size, this->size, 0, this->internalFormat, GL_UNSIGNED_BYTE, nullptr);
		}
		setFilter(LINEAR, LINEAR);
		setWrap(CLAMP_TO_EDGE, CLAMP_TO_EDGE, CLAMP_TO_EDGE);

		//  if (useMipMaps)
		//  {
		//  	glGenerateMipmap(GL_TEXTURE_2D);
		//  }
	});
}

GLsizei TextureCube::getSize() const
{
	return size;
}

PixelFormat TextureCube::getFormat() const
{
	return internalFormat;
}

void TextureCube::setWrap(Wrap s, Wrap t, Wrap r)
{
	temporaryBind([s, t, r]()
	{
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, s);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, t);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, r);
	});
}

template <typename Type>
void TextureCube::setData(CubeMapFace face, void* pixels, PixelFormat format, int size)
{
	temporaryBind([this, pixels, format, size]() {
		glTexImage2D(face, 0, internalFormat, size, size, 0, format, GLType<Type>(), pixels);
		this->size = size;
	});
}

template <typename Type>
void TextureCube::setData(CubeMapFace face, const void* pixels, PixelFormat format)
{
	temporaryBind([this, pixels, format]() {
		glTexImage2D(face, 0, internalFormat, this->size, this->size, 0, format, GLType<Type>(), pixels);
	});
}

template <typename Type>
void TextureCube::setSubData(CubeMapFace face, const void* pixels, PixelFormat format, int x, int y, int width, int height)
{
	temporaryBind([this, pixels, format, x, y, width, height]() {
		glTexSubImage2D(face, 0, x, y, width, height, format, GLType<Type>(), pixels);
	});
}

std::unique_ptr<TextureCube> TextureCube::fromFile(std::string right, std::string left, std::string top, std::string bottom, std::string back, std::string front)
{
	stbi_set_flip_vertically_on_load(false);

	std::unique_ptr<TextureCube> cubeMap = nullptr;

	std::string temp[] = { right, left, top, bottom, back, front };

	for (GLenum i = 0; i < 6; ++i)
	{
		std::string& path = temp[i];

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

		if(cubeMap == nullptr)
		{
			cubeMap = std::make_unique<TextureCube>(std::max(width, height), format);
			cubeMap->bind(0);
		}

		glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, 0, 0, width, height, format, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	return std::move(cubeMap);
}