#include "window.h"
#include "monitor.h"

void joystickCallback(int joystick, int event){
	switch (event){
		case GLFW_CONNECTED:
			break;
		case GLFW_DISCONNECTED:
			break;
		default:
			break;
	}
}
void monitorCallback(GLFWmonitor* monitor, int event){
	switch (event){
		case GLFW_CONNECTED:
			break;
		case GLFW_DISCONNECTED:
			break;
		default:
			break;
	}
}
void errorCallback(int error_code, const char* description){
	std::cerr << "Error Code " << error_code << " : " << description << std::endl;
}

void cursorPosCallback(GLFWwindow* window, double x, double y){
	Mouse::cursor_pos_callback(window, x, y);

	Window* win = (Window*)glfwGetWindowUserPointer(window);
	for (auto callback : win->cursorPosCallbacks){
		callback(win, x, y);
	}
}
void dropCallback(GLFWwindow* window, int count, const char ** names){
	std::vector<std::string> temp_names;
	for (int i = 0; i < count; ++i) {
		temp_names.push_back(std::string(names[i]));
	}

	Window* win = (Window*)glfwGetWindowUserPointer(window);
	for (auto callback : win->dropCallbacks){
		callback(win, temp_names);
	}
}
void scrollCallback(GLFWwindow* window, double xOffset, double yOffset){
	Window* win = (Window*)glfwGetWindowUserPointer(window);
	for (auto callback : win->scrollCallbacks){
		callback(win, xOffset, yOffset);
	}
}
void frameBufferSizeCallback(GLFWwindow* window, int width, int height){
	Window* win = (Window*)glfwGetWindowUserPointer(window);
	for (auto callback : win->frameBufferSizeCallbacks){
		callback(win, width, height);
	}
}
void charModsCallback(GLFWwindow* window, unsigned int codepoint, int mods){
	Window* win = (Window*)glfwGetWindowUserPointer(window);
	for (auto callback : win->charModsCallbacks){
		callback(win, codepoint, mods);
	}
}
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods){
	Mouse::mouse_button_callback(window, button, action, mods);

	Window* win = (Window*)glfwGetWindowUserPointer(window);
	for (auto callback : win->mouseButtonCallbacks){
		callback(win, (MouseButton)button, (Action)action, (Mod)mods);
	}
}
void windowPosCallback(GLFWwindow* window, int x, int y){
	Window* win = (Window*)glfwGetWindowUserPointer(window);
	win->x = x;
	win->y = y;
	for (auto callback : win->windowPosCallbacks){
		callback(win, x, y);
	}
}
void windowCloseCallback(GLFWwindow* window){
	Window* win = (Window*)glfwGetWindowUserPointer(window);
	for (auto callback : win->windowCloseCallbacks){
		callback(win);
	}
}
void windowRefreshCallback(GLFWwindow* window){
	Window* win = (Window*)glfwGetWindowUserPointer(window);
	for (auto callback : win->windowRefreshCallbacks){
		callback(win);
	}
}
void windowFocusCallback(GLFWwindow* window, int focused){
	Window* win = (Window*)glfwGetWindowUserPointer(window);
	for (auto callback : win->windowFocusCallbacks){
		callback(win, focused == GLFW_TRUE);
	}
}
void windowIconifyCallback(GLFWwindow* window, int iconified){
	Window* win = (Window*)glfwGetWindowUserPointer(window);
	for (auto callback : win->windowIconifyCallbacks){
		callback(win, iconified == GLFW_TRUE);
	}
}

void cursorEnterCallback(GLFWwindow* window, int entered) {
	Window* win = (Window*)glfwGetWindowUserPointer(window);
	for (auto callback : win->cursorEnterCallbacks){
		callback(win, entered == GLFW_TRUE);
	}
}
void charCallback(GLFWwindow* window, unsigned int character) {
	Window* win = (Window*)glfwGetWindowUserPointer(window);
	for (auto callback : win->charCallbacks){
		callback(win, character);
	}
}
void windowSizeCallback(GLFWwindow* window, int width, int height) {
	Window* win = (Window*)glfwGetWindowUserPointer(window);
	for (auto callback : win->windowSizeCallbacks){
		callback(win, width, height);
	}
}
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	Keyboard::key_callback(window, key, scancode, action, mods);

	Window* win = (Window*)glfwGetWindowUserPointer(window);
	for (auto callback : win->keyCallbacks){
		callback(win, Key(key), Action(action), Mod(mods));
	}
}


Window::Window(const std::string title, int width, int height) {
	this->title = title;
	this->width = width;
	this->height = height;

	if (!glfwInit()) {
		throw std::runtime_error("Could not init GLFW");
	}

	glfwDefaultWindowHints();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
	glfwWindowHint(GLFW_RESIZABLE, true);
	glfwWindowHint(GLFW_SAMPLES, 0);

	window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
	if (!window) {
		glfwTerminate();
		throw std::runtime_error("Could not create window");
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK) {
		glfwTerminate();
		throw std::runtime_error("Could not init GLEW");
	}

	const VideoMode mode = Monitor::getPrimaryMonitor().getVideoMode();
	glfwSetWindowPos(window, (mode.getWidth() - width) / 2, (mode.getHeight() - height) / 2);

	glfwGetWindowPos(window, &x, &y);
	glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);

	glfwSetWindowUserPointer(window, this);

	glfwSetErrorCallback(errorCallback);
	glfwSetMonitorCallback(monitorCallback);
	glfwSetJoystickCallback(joystickCallback);

	glfwSetKeyCallback(window, keyCallback);
	glfwSetCharCallback(window, charCallback);
	glfwSetDropCallback(window, dropCallback);
	glfwSetScrollCallback(window, scrollCallback);
	glfwSetCharModsCallback(window, charModsCallback);
	glfwSetCursorPosCallback(window, cursorPosCallback);
	glfwSetWindowPosCallback(window, windowPosCallback);
	glfwSetWindowSizeCallback(window, windowSizeCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetCursorEnterCallback(window, cursorEnterCallback);
	glfwSetWindowCloseCallback(window, windowCloseCallback);
	glfwSetWindowFocusCallback(window, windowFocusCallback);
	glfwSetWindowIconifyCallback(window, windowIconifyCallback);
	glfwSetWindowRefreshCallback(window, windowRefreshCallback);
	glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);
}

Window::~Window() {
	glfwDestroyWindow(window);
	glfwTerminate();
}

int Window::getWidth() const { return width; }

int Window::getHeight() const { return height; }

int Window::getFrameBufferWidth() const { return frameBufferWidth; }

int Window::getFrameBufferHeight() const { return frameBufferHeight; }

bool Window::shouldClose() const {
	return glfwWindowShouldClose(window) == GLFW_TRUE;
}

void Window::swapBuffers() const {
	glfwSwapBuffers(window);
}

void Window::pollEvents() const {
	glfwPollEvents();
}

void Window::setSwapInterval(int interval) const {
	glfwSwapInterval(interval);
}

Window* Window::getCurrentWindow() {
	return static_cast<Window*>(glfwGetWindowUserPointer(glfwGetCurrentContext()));
}

std::string Window::getClipboardString() const {
	return glfwGetClipboardString(window);
}

void Window::setClipboardString(const std::string string) const {
	glfwSetClipboardString(window, string.c_str());
}

void Window::iconify() const {
	glfwIconifyWindow(window);
}

void Window::restore() const {
	glfwRestoreWindow(window);
}

bool Window::isFocused() const {
	return glfwGetWindowAttrib(window, GLFW_FOCUSED) == GLFW_TRUE;
}

bool Window::isIconified() const {
	return glfwGetWindowAttrib(window, GLFW_ICONIFIED) == GLFW_TRUE;
}

bool Window::isResizable() const {
	return glfwGetWindowAttrib(window, GLFW_RESIZABLE) == GLFW_TRUE;
}

bool Window::isVisible() const {
	return glfwGetWindowAttrib(window, GLFW_VISIBLE) == GLFW_TRUE;
}

bool Window::isFullScreen() const {
	return glfwGetWindowMonitor(window) != nullptr;
}

void Window::setVisible(bool visible) const {
	if(visible){
		glfwShowWindow(window);
	}else{
		glfwHideWindow(window);
	}
}

void Window::setTitle(const std::string title) const {
	glfwSetWindowTitle(window, title.c_str());
//	this->title = std::string(title);
}

void Window::setSize(int width, int height) const {
	glfwSetWindowSize(window, width, height);
}

void Window::setWindowSizeLimits(int minWidth, int minHeight, int maxWidth, int maxHeight) const {
	glfwSetWindowSizeLimits(window, minWidth, minHeight, maxWidth, maxHeight);
}

void Window::setCursorPosition(double x, double y) const {
	glfwSetCursorPos(window, x, y);
}
