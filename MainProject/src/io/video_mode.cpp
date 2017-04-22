//
// Created by Dominic on 17/12/2016.
//

#include "video_mode.h"

VideoMode::VideoMode() : VideoMode(640, 360) {}

VideoMode::VideoMode(int width, int height) : VideoMode(width, height, 60) {}

VideoMode::VideoMode(int width, int height, int refreshrate) : VideoMode(width, height, 24, 24, 24, refreshrate) {}

VideoMode::VideoMode(int width, int height, int redbits, int greenbits, int bluebits) : VideoMode(width, height, redbits, greenbits, bluebits, 60) {}

VideoMode::VideoMode(int width, int height, int redbits, int greenbits, int bluebits, int refreshrate) {
	vidmode.width = width;
	vidmode.height = height;
	vidmode.redBits = redbits;
	vidmode.greenBits = greenbits;
	vidmode.blueBits = bluebits;
	vidmode.refreshRate = refreshrate;
}

VideoMode::VideoMode(GLFWvidmode vidmode) : vidmode(vidmode) {}

int VideoMode::getWidth() const {
	return vidmode.width;
}

void VideoMode::setWidth(int width) {
	vidmode.width = width;
}

int VideoMode::getHeight() const {
	return vidmode.height;
}

void VideoMode::setHeight(int height) {
	vidmode.height = height;
}

int VideoMode::getRedBits() const {
	return vidmode.redBits;
}

void VideoMode::setRedBits(int redbits) {
	vidmode.redBits = redbits;
}

int VideoMode::getGreenBits() const {
	return vidmode.greenBits;
}

void VideoMode::setGreenBits(int greenbits) {
	vidmode.greenBits = greenbits;
}

int VideoMode::getBlueBits() const {
	return vidmode.blueBits;
}

void VideoMode::setBlueBits(int bluebits) {
	vidmode.blueBits = bluebits;
}

int VideoMode::getRefreshRate() const {
	return vidmode.refreshRate;
}

void VideoMode::setRefreshRate(int refreshrate) {
	vidmode.refreshRate = refreshrate;
}

//VideoMode::operator GLFWvidmode*() const {
//	return &vidmode;
//}
