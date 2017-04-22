//
// Created by Dominic on 17/12/2016.
//

#include <GLFW/glfw3.h>

enum class Action : unsigned int
{
	Press = GLFW_PRESS,
	Release = GLFW_RELEASE,
	Repeat = GLFW_REPEAT
};

enum class Mod : unsigned int
{
	Shift = GLFW_MOD_SHIFT,
	Control = GLFW_MOD_CONTROL,
	Alt = GLFW_MOD_ALT,
	Super = GLFW_MOD_SUPER
};
