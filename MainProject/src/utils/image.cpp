//
// Created by Dominic on 15/12/2016.
//

#include "image.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <sstream>

Image::Image(std::string file) {
	stbi_set_flip_vertically_on_load(true);

	int w, h, components;
	unsigned char* data = stbi_load(file.c_str(), &w, &h, &components, STBI_rgb_alpha);

	width = static_cast<unsigned int>(w);
	height = static_cast<unsigned int>(h);
	pixels = new Color[width * height];

	const int size = (width * height) / sizeof(Color);
	for (int i = 0; i < size; ++i) {
		pixels[i] = Color(
				data[(i * 4) + 0],
				data[(i * 4) + 1],
				data[(i * 4) + 2],
				data[(i * 4) + 3]
		);
	}

	stbi_image_free(data);
}

Image::Image(size_t width, size_t height, const Color &color) {
	this->width = width;
	this->height = height;

	pixels = new Color[width * height];
	for (size_t x = 0; x < width; ++x) {
		for (size_t y = 0; y < height; ++y) {
			pixels[(x * width) + y] = color;
		}
	}
}

Image::~Image() {
	delete pixels;
}

void Image::setPixel(unsigned int x, unsigned int y, const Color &color) {
	if(x < 0 || x >= width || y < 0 || y >= height){
		std::ostringstream buffer;
		buffer << "Tried to access (" << x << ", " << y << ") from an image of size [" << width << ", " << height << "]";
		throw std::out_of_range(buffer.str());
	}

	pixels[(x * width) + y] = color;
}

const Color& Image::getPixel(unsigned int x, unsigned int y) const {
	if(x < 0 || x >= width || y < 0 || y >= height){
		std::ostringstream buffer;
		buffer << "Tried to access (" << x << ", " << y << ") from an image of size [" << width << ", " << height << "]";
		throw std::out_of_range(buffer.str());
	}

	return pixels[(x * width) + y];
}
