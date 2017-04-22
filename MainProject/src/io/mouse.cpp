#include "mouse.h"

float Mouse::x = 0;
float Mouse::y = 0;
bool Mouse::buttonStates[GLFW_MOUSE_BUTTON_LAST];

float Mouse::getX() {
	return x;
}

float Mouse::getY() {
	return y;
}

bool Mouse::isButtonPressed(MouseButton button) {
	return buttonStates[(int)button];
}

bool Mouse::isButtonDown(MouseButton button) {
	return buttonStates[(int)button];
}

bool Mouse::isButtonUp(MouseButton button) {
	return !buttonStates[(int)button];
}

void Mouse::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	buttonStates[button] = action != GLFW_RELEASE;
}

void Mouse::cursor_pos_callback(GLFWwindow* window, double x, double y) {
	Mouse::x = (float) x;
	Mouse::y = (float) y;
}
