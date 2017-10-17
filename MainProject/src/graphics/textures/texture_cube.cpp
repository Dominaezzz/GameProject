#include "texture_cube.h"
#include "../../utils/ext/stb_image.h"

TextureCube::TextureCube(GLsizei size, PixelFormat internalFormat) : size(size), internalFormat(internalFormat)
{
#ifdef USE_OPENGL_DSA
	glTextureStorage2D(this->texture, 1, GL_RGBA8, size, size);
	setFilter(MinFilter::LINEAR, MagFilter::LINEAR);
	setWrap(CLAMP_TO_EDGE, CLAMP_TO_EDGE, CLAMP_TO_EDGE);
#else
	temporaryBind([this]() {
		for (GLenum i = GL_TEXTURE_CUBE_MAP_POSITIVE_X; i <= GL_TEXTURE_CUBE_MAP_NEGATIVE_Z; ++i)
		{
			glTexImage2D(i, 0, this->internalFormat, this->size, this->size, 0, this->internalFormat, GL_UNSIGNED_BYTE, nullptr);
		}
		setFilter(MinFilter::LINEAR, MagFilter::LINEAR);
		setWrap(CLAMP_TO_EDGE, CLAMP_TO_EDGE, CLAMP_TO_EDGE);
	});
#endif
}

GLsizei TextureCube::getSize() const
{
	return size;
}

PixelFormat TextureCube::getFormat() const
{
	return internalFormat;
}

void TextureCube::setWrap(const Wrap s, const Wrap t, const Wrap r)
{
#ifdef USE_OPENGL_DSA
	glTextureParameteri(this->getId(), GL_TEXTURE_WRAP_S, s);
	glTextureParameteri(this->getId(), GL_TEXTURE_WRAP_T, t);
	glTextureParameteri(this->getId(), GL_TEXTURE_WRAP_R, r);
#else
	temporaryBind([s, t, r]()
	{
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, s);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, t);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, r);
	});
#endif
}

template <typename Type>
void TextureCube::setData(CubeMapFace face, const void* pixels, PixelFormat format)
{
	setSubData<Type>(face, pixels, format, 0, 0, this->size, this->size);
}

template <typename Type>
void TextureCube::setSubData(CubeMapFace face, const void* pixels, PixelFormat format, int x, int y, int width, int height)
{
#ifdef USE_OPENGL_DSA
	glTextureSubImage3D(getId(), 0, x, y, face - CubeMapFace::Right, width, height, 1, format, GLType<Type>(), pixels);
#else
	temporaryBind([this, pixels, format, x, y, width, height]() {
		glTexSubImage2D(face, 0, x, y, width, height, format, GLType<Type>(), pixels);
	});
#endif
}

std::unique_ptr<TextureCube> TextureCube::fromFile(const std::string& right, const std::string& left, const std::string& top, const std::string& bottom, const std::string
												   & back, const std::string& front)
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
		}
		cubeMap->setData<unsigned char>(static_cast<CubeMapFace>(CubeMapFace::Right + i), data, format);

		stbi_image_free(data);
	}

	return std::move(cubeMap);
}