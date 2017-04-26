#pragma once

#include <string>
#include <stdexcept>
#include "color.h"
#include <vector>

class Image
{
	std::vector<unsigned char> pixelData;
	unsigned int width, height, components;
public:
	explicit Image(std::string file);
	Image(size_t width, size_t height, const Color& color = Color());
	unsigned char* operator &();
	void setPixel(unsigned int x, unsigned int y, const Color& color);
	Color getPixel(unsigned int x, unsigned int y) const;
};
