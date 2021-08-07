// Project: libv.frame, File: src/libv/frame/monitor.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <libv/frame/monitor.hpp>
// ext
#include <GLFW/glfw3.h>
// std
#include <cassert>
#include <limits>


namespace libv {
namespace frame {

// -------------------------------------------------------------------------------------------------

std::mutex Monitor::monitors_m;
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

Monitor& Monitor::getMonitorClosest(libv::vec2i coord) {
	std::lock_guard lock(Monitor::monitors_m);
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
	std::lock_guard lock(Monitor::monitors_m);
	return monitors.at(glfwGetPrimaryMonitor());
}

Monitor::Monitor(GLFWmonitor* monitor) {
	handler = monitor;
	const auto* currentMode = glfwGetVideoMode(monitor);
	currentVideoMode.colorBits = libv::vec3i(currentMode->redBits, currentMode->greenBits, currentMode->blueBits);
	currentVideoMode.refreshRate = currentMode->refreshRate;
	currentVideoMode.size = libv::vec2i(currentMode->width, currentMode->height);

	name = glfwGetMonitorName(monitor);
	glfwGetMonitorContentScale(monitor, &contentScale.x, &contentScale.y);
	glfwGetMonitorPhysicalSize(monitor, &physicalSizeMM.x, &physicalSizeMM.y);
	glfwGetMonitorPos(monitor, &position.x, &position.y);
	glfwGetMonitorWorkarea(monitor, &workAreaPosition.x, &workAreaPosition.y, &workAreaSize.x, &workAreaSize.y);

	int n;
	auto modes = glfwGetVideoModes(monitor, &n);
	videoModes.reserve(n);
	for (int i = 0; i < n; i++) {
		auto& mode = videoModes.emplace_back();

		mode.colorBits = libv::vec3i(modes[i].redBits, modes[i].greenBits, modes[i].blueBits);
		mode.refreshRate = modes[i].refreshRate;
		mode.size = libv::vec2i(modes[i].width, modes[i].height);
	}
}

void dispatchGLFWMonitorEvent(GLFWmonitor* monitor, int status) {
	std::lock_guard lock(Monitor::monitors_m);

	Monitor* m;
	try {
		m = &Monitor::monitors.at(monitor);
	} catch (const std::out_of_range&) {
		m = &Monitor::monitors.emplace(monitor, monitor).first->second;
	}
	Monitor::onMonitor.fire(EventMonitor(*m, status));
}

// -------------------------------------------------------------------------------------------------

} // namespace frame
} // namespace libv
