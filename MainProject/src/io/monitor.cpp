//
// Created by Dominic on 17/12/2016.
//

#include "monitor.h"

Monitor::Monitor(GLFWmonitor* monitor) : monitor(monitor) {}

const std::vector<Monitor> Monitor::getMonitors() {
	int count;
	GLFWmonitor** monitors = glfwGetMonitors(&count);

	std::vector<Monitor> result;
	for (int i = 0; i < count; ++i) {
		result.push_back(Monitor(monitors[i]));
	}
	return result;
}

const Monitor Monitor::getPrimaryMonitor() {
	return Monitor(glfwGetPrimaryMonitor());
}

const std::string Monitor::getName() const {
	return glfwGetMonitorName(monitor);
}

const std::vector <VideoMode> Monitor::getVideoModes() const {
	int count;
	const GLFWvidmode* videoModes = glfwGetVideoModes(monitor, &count);

	std::vector<VideoMode> result((unsigned int) count);
	for (int i = 0; i < count; ++i) {
		result.push_back(VideoMode(videoModes[i]));
	}
	return result;
}

const VideoMode Monitor::getVideoMode() const {
	return VideoMode(*glfwGetVideoMode(monitor));
}

void Monitor::setGamma(float gamma) {
	glfwSetGamma(monitor, gamma);
}

const GammaRamp Monitor::getGammaRamp() const {
	return GammaRamp(*glfwGetGammaRamp(monitor));
}

void Monitor::setGammaRamp(GammaRamp &ramp) {
	glfwSetGammaRamp(monitor, ramp);
}

Monitor::operator GLFWmonitor*() const {
	return monitor;
}

const MonitorPos Monitor::getPos() const {
	MonitorPos pos;
	glfwGetMonitorPos(monitor, &pos.x, &pos.y);
	return pos;
}

const MonitorSize Monitor::getPhysicalSize() const {
	MonitorSize size;
	glfwGetMonitorPhysicalSize(monitor, &size.width, &size.height);
	return size;
}
