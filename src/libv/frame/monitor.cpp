// File: monitor.cpp, Created on 2015. április 9. 20:44, Author: Vader

// hpp
#include <libv/frame/monitor.hpp>
// ext
#include <GLFW/glfw3.h>
// std
#include <limits>


namespace libv {
namespace frame {

// -------------------------------------------------------------------------------------------------

VideoMode::VideoMode(const GLFWvidmode* vidmode) :
	colorBits(vidmode->redBits, vidmode->greenBits, vidmode->blueBits),
	refreshRate(vidmode->refreshRate),
	size(vidmode->width, vidmode->height) { }

VideoMode& VideoMode::operator=(const GLFWvidmode* vidmode) & {
	colorBits = vec3i(vidmode->redBits, vidmode->greenBits, vidmode->blueBits);
	refreshRate = vidmode->refreshRate;
	size = vec2i(vidmode->width, vidmode->height);
	return *this;
}

// -------------------------------------------------------------------------------------------------

void glfwMonitorCallback(GLFWmonitor* monitor, int status) {
	Monitor* m;
	try {
		m = &Monitor::monitors.at(monitor);
	} catch (const std::out_of_range&) {
		m = &Monitor::monitors.emplace(monitor, monitor).first->second;
	}
	Monitor::onMonitor.fire(EventMonitor(*m, status));
}

std::map<GLFWmonitor*, Monitor> Monitor::monitors;
Signal<const EventMonitor&> Monitor::onMonitor;

template <typename T>
T distFromSection(T a, T b, T p) {
	if (p < a)
		return a - p;
	else if (p > b)
		return p - b;
	else
		return 0;
}

Monitor& Monitor::getMonitorAt(vec2i coord) {
	// TODO P5: std::min_element with lambda
	int min = std::numeric_limits<int>::max();

	Monitor* minid = nullptr;
	for (auto& monitor : monitors) {
		auto xdist = distFromSection(monitor.second.position.x, monitor.second.position.x + monitor.second.currentVideoMode.size.x, coord.x);
		auto ydist = distFromSection(monitor.second.position.y, monitor.second.position.y + monitor.second.currentVideoMode.size.y, coord.y);
		auto dist = xdist * xdist + ydist * ydist;

		if (dist < min) {
			min = dist;
			minid = &monitor.second;
		}
	}
	assert(minid != nullptr);
	return *minid;
}

Monitor& Monitor::getPrimaryMonitor() {
	return monitors.at(glfwGetPrimaryMonitor());
}

Monitor::Monitor(GLFWmonitor* monitor) {
	currentVideoMode = glfwGetVideoMode(monitor);
	name = glfwGetMonitorName(monitor);
	glfwGetMonitorContentScale(monitor, &contentScale.x, &contentScale.y);
	glfwGetMonitorPhysicalSize(monitor, &physicalSizeMM.x, &physicalSizeMM.y);
	glfwGetMonitorPos(monitor, &position.x, &position.y);
	glfwGetMonitorWorkarea(monitor, &workAreaPosition.x, &workAreaPosition.y, &workAreaSize.x, &workAreaSize.y);

	int n;
	auto modes = glfwGetVideoModes(monitor, &n);
	videoModes.reserve(n);
	for (int i = 0; i < n; i++)
		videoModes.emplace_back(&modes[i]);
}

// -------------------------------------------------------------------------------------------------

} // namespace frame
} // namespace libv
