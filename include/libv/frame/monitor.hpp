// File: monitor.hpp, Created on 2015. április 9. 20:44, Author: Vader

#pragma once

// libv
#include <libv/math/vec.hpp>
#include <libv/sig/signal.hpp>
// std
#include <vector>
// pro
#include <libv/frame/events.hpp>


class GLFWvidmode;
class GLFWmonitor;

namespace libv {
namespace frame {

// -------------------------------------------------------------------------------------------------

struct VideoMode {
	libv::vec3i colorBits;
	int refreshRate;
	libv::vec2i size;

	VideoMode() = default;
	VideoMode(const GLFWvidmode* vidmode);
	VideoMode& operator=(const GLFWvidmode* vidmode) &;
	~VideoMode() = default;
};

// -------------------------------------------------------------------------------------------------

void glfwMonitorCallback(GLFWmonitor*, int);

struct Monitor {
	static std::map<GLFWmonitor*, Monitor> monitors;
	static Signal<const EventMonitor&> onMonitor;

public:
	static Monitor& getMonitorAt(libv::vec2i coord);
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

public:
	Monitor(GLFWmonitor* monitor);
};

// -------------------------------------------------------------------------------------------------

} // namespace frame
} // namespace libv
