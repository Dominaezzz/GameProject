#pragma once

#include <GLFW/glfw3.h>

enum class MouseButton : int
{
	_1 = GLFW_MOUSE_BUTTON_1,
	_2 = GLFW_MOUSE_BUTTON_2,
	_3 = GLFW_MOUSE_BUTTON_3,
	_4 = GLFW_MOUSE_BUTTON_4,
	_5 = GLFW_MOUSE_BUTTON_5,
	_6 = GLFW_MOUSE_BUTTON_6,
	_7 = GLFW_MOUSE_BUTTON_7,
	_8 = GLFW_MOUSE_BUTTON_8,
	LAST = GLFW_MOUSE_BUTTON_LAST,
	LEFT = GLFW_MOUSE_BUTTON_LEFT,
	RIGHT = GLFW_MOUSE_BUTTON_RIGHT,
	MIDDLE = GLFW_MOUSE_BUTTON_MIDDLE
};

class Mouse
{
private:
	Mouse() {}
	~Mouse() {}
	static float x, y;
	static bool buttonStates[];
public:
	static float getX();
	static float getY();
	static bool isButtonPressed(MouseButton button);
	static bool isButtonDown(MouseButton button);
	static bool isButtonUp(MouseButton button);
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	static void cursor_pos_callback(GLFWwindow* window, double x, double y);
};
