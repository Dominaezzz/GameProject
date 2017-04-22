#pragma once

#include <string>
#include <stdexcept>
#include "color.h"

class Image
{
	Color* pixels;
	unsigned int height, width;
public:
	Image(std::string file);
	Image(unsigned int width, unsigned int height, const Color& color = Color());
	~Image();
	void setPixel(unsigned int x, unsigned int y, const Color& color);
	const Color& getPixel(unsigned int x, unsigned int y) const;
};
