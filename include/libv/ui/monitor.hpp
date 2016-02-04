// File: monitor.hpp, Created on 2015. április 9. 20:44, Author: Vader

#pragma once

// libv
#include <libv/vec.hpp>
#include <libv/sig/signal.hpp>
// std
#include <vector>
// pro
#include <libv/ui/events/events.hpp>

class GLFWvidmode;
class GLFWmonitor;

namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

struct VideoMode {
public:
	ivec3 colorBits;
	int refreshRate;
	ivec2 size;

public:
	VideoMode();
	VideoMode(const GLFWvidmode* vidmode);
	VideoMode& operator=(const GLFWvidmode* vidmode);
	virtual ~VideoMode();
};

// -------------------------------------------------------------------------------------------------

void glfwMonitorCallback(GLFWmonitor*, int);

struct Monitor {
public:
	static std::map<GLFWmonitor*, Monitor> monitors;
	static Monitor* getMonitorAt(ivec2 coord);
	static Signal<const EventMonitor&> onMonitor;

public:
	std::string name;
	ivec2 position;
	VideoMode currentVideoMode;
	std::vector<VideoMode> videoModes;

public:
	Monitor(GLFWmonitor* monitor);
};

// -------------------------------------------------------------------------------------------------

} //namespace ui
} //namespace libv