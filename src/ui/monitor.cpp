// File: monitor.cpp, Created on 2015. április 9. 20:44, Author: Vader

// hpp
#include <vl/ui/monitor.hpp>
// ext
#include <GLFW/glfw3.h>
// std
#include <limits>


namespace vl {
namespace ui {

// -------------------------------------------------------------------------------------------------

VideoMode::VideoMode() { }

VideoMode::VideoMode(const GLFWvidmode* vidmode) :
	colorBits(vidmode->redBits, vidmode->greenBits, vidmode->blueBits),
	refreshRate(vidmode->refreshRate),
	size(vidmode->width, vidmode->height) { }

VideoMode& VideoMode::operator=(const GLFWvidmode* vidmode) {
	colorBits = ivec3(vidmode->redBits, vidmode->greenBits, vidmode->blueBits);
	refreshRate = vidmode->refreshRate;
	size = ivec2(vidmode->width, vidmode->height);
	return *this;
}

VideoMode::~VideoMode() { }

// -------------------------------------------------------------------------------------------------

void glfwMonitorCallback(GLFWmonitor* monitor, int status) {
	Monitor* m;
	try {
		m = &Monitor::monitors.at(monitor);
	} catch (const std::out_of_range&) {
		m = &Monitor::monitors.emplace(monitor, monitor).first->second;
	}
	Monitor::onMonitor.fire(EventMonitor(m, status));
}

std::map<GLFWmonitor*, Monitor> Monitor::monitors;
Signal<const EventMonitor&> Monitor::onMonitor;

int distFromSection(int a, int b, int p) {
	if (p < a)
		return a - p;
	else if (p > b)
		return p - b;
	else
		return 0;
}

Monitor* Monitor::getMonitorAt(const ivec2& coord) {
	//TODO P5: std::min_element with lambda
	int min = std::numeric_limits<int>::max();

	Monitor* minid = nullptr;
	for (auto& monitor : monitors) {
		int xdist = distFromSection(monitor.second.position.x, monitor.second.position.x + monitor.second.currentVideoMode.size.x, coord.x);
		int ydist = distFromSection(monitor.second.position.y, monitor.second.position.y + monitor.second.currentVideoMode.size.y, coord.y);
		int dist = xdist * xdist + ydist * ydist;

		if (dist < min) {
			min = dist;
			minid = &monitor.second;
		}
	}

	//??? assert nullptr
	return minid;
}

Monitor::Monitor(GLFWmonitor* monitor) {
	currentVideoMode = glfwGetVideoMode(monitor);
	name = glfwGetMonitorName(monitor);
	glfwGetMonitorPos(monitor, &position.x, &position.y);
	int n;
	auto modes = glfwGetVideoModes(monitor, &n);
	videoModes.reserve(n);
	for (int i = 0; i < n; i++)
		videoModes.emplace_back(&modes[i]);
}

// -------------------------------------------------------------------------------------------------

} //namespace ui
} //namespace vl