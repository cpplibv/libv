// File: monitor.hpp, Created on 2015. április 9. 20:44, Author: Vader

#pragma once

// libv
#include <libv/math/vec.hpp>
#include <libv/sig/signal.hpp>
// std
#include <mutex>
#include <vector>
// pro
#include <libv/frame/events.hpp>

// -------------------------------------------------------------------------------------------------

class GLFWmonitor;

namespace libv {
namespace frame {

struct VideoMode {
	libv::vec3i colorBits;
	int refreshRate;
	libv::vec2i size;
};

struct Monitor {
	static std::mutex monitors_m;
	static std::map<GLFWmonitor*, Monitor> monitors;
	static Signal<const EventMonitor&> onMonitor;

public:
	static Monitor& getMonitorClosest(libv::vec2i coord);
	static Monitor& getPrimaryMonitor();

public:
	std::string name;
	libv::vec2i position;
	libv::vec2i physicalSizeMM;
	libv::vec2i workAreaPosition;
	libv::vec2i workAreaSize;
	libv::vec2f contentScale;

	VideoMode currentVideoMode;
	std::vector<VideoMode> videoModes;

	GLFWmonitor* handler;
public:
	Monitor(GLFWmonitor* monitor);
};

void dispatchGLFWMonitorEvent(GLFWmonitor* monitor, int status);

} // namespace frame
} // namespace libv

// -------------------------------------------------------------------------------------------------
