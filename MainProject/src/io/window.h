#pragma once

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <functional>
#include <algorithm>
#include "keyboard.h"
#include "mouse.h"
#include "input.h"

class Window;

typedef std::function<void (Window*, Key, Action, Mod)> KeyCallback;
typedef std::function<void (Window*, int, int)> WindowSizeCallback;
typedef std::function<void (Window*, unsigned int)> CharCallback;

typedef std::function<void (Window*)> WindowCloseCallback;
typedef std::function<void (Window*)> WindowRefreshCallback;
typedef std::function<void (Window*, bool)> WindowFocusCallback;
typedef std::function<void (Window*, bool)> WindowIconifyCallback;
typedef std::function<void (Window*, int, int)> WindowPosCallback;
typedef std::function<void (Window*, double, double)> ScrollCallback;
typedef std::function<void (Window*, bool entered)> CursorEnterCallback;
typedef std::function<void (Window*, double, double)> CursorPosCallback;
typedef std::function<void (Window*, unsigned int, int)> CharModsCallback;
typedef std::function<void (Window*, double, double)> FrameBufferSizeCallback;
typedef std::function<void (Window*, const std::vector<std::string>)> DropCallback;
typedef std::function<void (Window*, MouseButton, Action, Mod)> MouseButtonCallback;

class Window
{
	GLFWwindow* window;
	std::string title;
	int x, y, width, height;
	int frameBufferWidth, frameBufferHeight;
	std::vector<WindowPosCallback> windowPosCallbacks;
	std::vector<WindowCloseCallback> windowCloseCallbacks;
	std::vector<WindowRefreshCallback> windowRefreshCallbacks;
	std::vector<WindowFocusCallback> windowFocusCallbacks;
	std::vector<WindowIconifyCallback> windowIconifyCallbacks;
	std::vector<ScrollCallback> scrollCallbacks;
	std::vector<CursorEnterCallback> cursorEnterCallbacks;
	std::vector<CursorPosCallback> cursorPosCallbacks;
	std::vector<CharModsCallback> charModsCallbacks;
	std::vector<MouseButtonCallback> mouseButtonCallbacks;
	std::vector<FrameBufferSizeCallback> frameBufferSizeCallbacks;
	std::vector<DropCallback> dropCallbacks;
	std::vector<KeyCallback> keyCallbacks;
	std::vector<CharCallback> charCallbacks;
	std::vector<WindowSizeCallback> windowSizeCallbacks;
public:
	Window(const std::string title, int width, int height);
	~Window();
	int getWidth() const;
	int getHeight() const;
	int getFrameBufferWidth() const;
	int getFrameBufferHeight() const;
	bool shouldClose() const;
	void swapBuffers() const;
	void pollEvents() const;
	std::string getClipboardString() const;
	void setClipboardString(const std::string string) const;
	void setTitle(const std::string title) const;
	void setSize(int width, int height) const;
	void restore() const;
	void iconify() const;
	bool isVisible() const;
	bool isFocused() const;
	bool isIconified() const;
	bool isResizable() const;
	bool isFullScreen() const;
	void setVisible(bool visible) const;
	void setSwapInterval(int interval) const;
	void setCursorPosition(double x, double y) const;
	void setWindowSizeLimits(int minWidth, int minHeight, int maxWidth, int maxHeight) const;

	void addWindowSizeCallback(WindowSizeCallback callback){ windowSizeCallbacks.push_back(callback); }

	inline static Window* getCurrentWindow();

private:
	friend void cursorEnterCallback(GLFWwindow* window, int entered);
	friend void charCallback(GLFWwindow* window, unsigned int character);
	friend void windowSizeCallback(GLFWwindow* window, int width, int height);
	friend void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	friend void cursorPosCallback(GLFWwindow* window, double x, double y);
	friend void dropCallback(GLFWwindow* window, int count, const char ** names);
	friend void scrollCallback(GLFWwindow* window, double xOffset, double yOffset);
	friend void frameBufferSizeCallback(GLFWwindow* window, int width, int height);
	friend void charModsCallback(GLFWwindow* window, unsigned int codepoint, int mods);
	friend void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	friend void windowPosCallback(GLFWwindow* window, int x, int y);
	friend void windowCloseCallback(GLFWwindow* window);
	friend void windowRefreshCallback(GLFWwindow* window);
	friend void windowFocusCallback(GLFWwindow* window, int focused);
	friend void windowIconifyCallback(GLFWwindow* window, int iconified);
};
