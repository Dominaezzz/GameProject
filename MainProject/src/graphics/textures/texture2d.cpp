#include "texture2d.h"
#include "../../utils/ext/stb_image.h"


Texture2D::Texture2D(const GLsizei width, const GLsizei height, const PixelFormat internalFormat) : width(width), height(height), internalFormat(internalFormat)
{
#ifdef USE_OPENGL_DSA
	glTextureStorage2D(this->texture, 1, GL_RGBA8, width, height);
	setFilter(MinFilter::LINEAR, MagFilter::LINEAR);
	setWrap(REPEAT, REPEAT);
#else
	temporaryBind([this]() {
		setFilter(MinFilter::LINEAR, MagFilter::LINEAR);
		setWrap(REPEAT, REPEAT);

		glTexImage2D(GL_TEXTURE_2D, 0, this->internalFormat, this->width, this->height, 0, this->internalFormat, GL_UNSIGNED_BYTE, nullptr);
	});
#endif
}

GLsizei Texture2D::getWidth() const
{
	return width;
}

GLsizei Texture2D::getHeight() const
{
	return height;
}

PixelFormat Texture2D::getFormat() const
{
	return internalFormat;
}

void Texture2D::setWrap(const Wrap s, const Wrap t)
{
#ifdef USE_OPENGL_DSA
	glTextureParameteri(this->texture, GL_TEXTURE_WRAP_S, s);
	glTextureParameteri(this->texture, GL_TEXTURE_WRAP_T, t);
#else
		temporaryBind([s, t]() {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, s);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, t);
		});
#endif
}

template<typename Type>
void Texture2D::setData(const void* pixels, const PixelFormat format) {
	setSubData<Type>(pixels, format, 0, 0, width, height);
}

template<typename Type>
void Texture2D::setSubData(const void* pixels, PixelFormat format, int x, int y, int width, int height) {
#ifdef USE_OPENGL_DSA
	glTextureSubImage2D(this->texture, 0, x, y, width, height, format, GLType<Type>(), pixels);
#else
	temporaryBind([this, pixels, format, x, y, width, height]() {
		glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, width, height, format, GLType<Type>(), pixels);
	});
#endif
}

std::shared_ptr<Texture2D> Texture2D::fromFile(const std::string& path, const bool useMipMaps) {
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

	if (useMipMaps)
	{
		texture->generateMipmap();
		texture->setFilter(MinFilter::LINEAR_MIPMAP_LINEAR, MagFilter::LINEAR);
	}

	stbi_image_free(data);

	return texture;
}
