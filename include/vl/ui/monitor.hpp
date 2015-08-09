// File: monitor.hpp, Created on 2015. április 9. 20:44, Author: Vader

#pragma once

// vl
#include <vl/vec.hpp>
#include <vl/sig/signal.hpp>
// std
#include <vector>
// pro
#include <vl/ui/events/events.hpp>

class GLFWvidmode;
class GLFWmonitor;

namespace vl {
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
	static Monitor* getMonitorAt(const ivec2& coord);
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
} //namespace vl