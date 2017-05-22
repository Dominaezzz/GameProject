//
// Created by Dominic on 15/12/2016.
//

#include "image.h"
#define STB_IMAGE_IMPLEMENTATION
#include "ext/stb_image.h"
#include <sstream>

Image::Image(std::string file) {
	stbi_set_flip_vertically_on_load(true);

	int w, h, c;
	unsigned char* data = stbi_load(file.c_str(), &w, &h, &c, STBI_rgb_alpha);

	width = static_cast<unsigned int>(w);
	height = static_cast<unsigned int>(h);
	components = static_cast<unsigned int>(c);

	pixelData.insert(pixelData.begin(), w * h * c, data[0]);

	stbi_image_free(data);
}

Image::Image(size_t width, size_t height, const Color &color) : pixelData(width * height * 4), width(width), height(height), components(4)
{
	for (size_t i = 0; i < width * height * components; i += 4) {
		pixelData[i + 0] = color.r;
		pixelData[i + 1] = color.g;
		pixelData[i + 2] = color.b;
		pixelData[i + 3] = color.a;
	}
}

unsigned char * Image::operator&()
{
	return pixelData.data();
}

void Image::setPixel(unsigned int x, unsigned int y, const Color &color) {
	if(x < 0 || x >= width || y < 0 || y >= height){
		std::ostringstream buffer;
		buffer << "Tried to access (" << x << ", " << y << ") from an image of size [" << width << ", " << height << "]";
		throw std::out_of_range(buffer.str());
	}

	size_t index = (x * width) + y;
	switch (components)
	{
	case 4:
		pixelData[index + 3] = color.a;
	case 3:
		pixelData[index + 2] = color.b;
	case 2:
		pixelData[index + 1] = color.g;
	case 1:
		pixelData[index + 0] = color.r;
		break;
	default:
		throw std::runtime_error("WTF!!!");
	}
}

Color Image::getPixel(unsigned int x, unsigned int y) const {
	if(x < 0 || x >= width || y < 0 || y >= height){
		std::ostringstream buffer;
		buffer << "Tried to access (" << x << ", " << y << ") from an image of size [" << width << ", " << height << "]";
		throw std::out_of_range(buffer.str());
	}

	Color color;
	size_t index = (x * width) + y;
	switch (components)
	{
	case 4:
		color.a = pixelData[index + 3];
	case 3:
		color.b = pixelData[index + 2];
	case 2:
		color.g = pixelData[index + 1];
	case 1:
		color.r = pixelData[index + 0];
		break;
	default:
		throw std::runtime_error("WTF!!!");
	}
	return color;
}
