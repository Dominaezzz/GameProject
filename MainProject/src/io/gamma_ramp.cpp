//
// Created by Dominic on 17/12/2016.
//

#include "gamma_ramp.h"


GammaRamp::GammaRamp(GLFWgammaramp gammaramp) : gammaramp(gammaramp) {}

GammaRamp::GammaRamp() {}

unsigned short* GammaRamp::getRed() {
	return gammaramp.red;
}

void GammaRamp::setRed(unsigned short* red) {
	gammaramp.red = red;
}

unsigned short* GammaRamp::getGreen() {
	return gammaramp.green;
}

void GammaRamp::setGreen(unsigned short* green) {
	gammaramp.green = green;
}

unsigned short* GammaRamp::getBlue() {
	return gammaramp.blue;
}

void GammaRamp::setBlue(unsigned short* blue) {
	gammaramp.blue = blue;
}

unsigned int GammaRamp::getSize() {
	return gammaramp.size;
}

void GammaRamp::setSize(unsigned int size) {
	gammaramp.size = size;
}

GammaRamp::operator GLFWgammaramp*(){
	return &gammaramp;
}
