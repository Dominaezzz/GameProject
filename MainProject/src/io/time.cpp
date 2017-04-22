//
// Created by Dominic on 17/12/2016.
//

#include "time.h"

double Time::get() {
	return glfwGetTime();
}

void Time::set(double time) {
	glfwSetTime(time);
}
