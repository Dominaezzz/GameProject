#include "keyboard.h"

bool Keyboard::keyStates[GLFW_KEY_LAST];

bool Keyboard::isKeyPressed(Key key) {
	return keyStates[(int)key];
}

bool Keyboard::isKeyDown(Key key) {
	return keyStates[(int)key];
}

bool Keyboard::isKeyUp(Key key) {
	return !keyStates[(int)key];
}

void Keyboard::key_callback(GLFWwindow * window, int key, int scancode, int action, int mods) {
	if (key >= 0)keyStates[key] = action != GLFW_RELEASE;
}
