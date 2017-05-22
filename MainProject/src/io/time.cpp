#include <GLFW/glfw3.h>
#include "time.h"

double Time::get()
{
	return glfwGetTime();
}

void Time::set(double time)
{
	glfwSetTime(time);
}
