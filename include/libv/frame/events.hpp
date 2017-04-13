// File: Events.hpp, Created on 2013. december 31. 21:52, Author: Vader

#pragma once

// ext
#include <boost/algorithm/string/join.hpp> // toPrettyString
#include <fmt/format.h> // toPrettyString
// libv
#include <libv/utf8.hpp>
#include <libv/math/vec.hpp>
// std
#include <cstring>
#include <string>
#include <vector>
// pro
#include <libv/frame/inputs.hpp>
#include <libv/utility/utility.hpp>

// TODO P5: Most of the event should get a observer_ptr<Frame> as member
// TODO P5: Review every event and change int to bool or enum, and (double, double) to vec2d
// TODO P5: Remove? EventWindowRefresh and EventWindowClose as they are handled by frame
//			EventWindowRefresh is not that simple, i think i have to work with it to 'force' refresh
//			if context is frozen due to event qua 'lock' in glfw (moving window)

namespace libv {
namespace frame {

// -------------------------------------------------------------------------------------------------

class Monitor;
class Frame;

struct EventChar {
	uint32_t unicode;
	char utf8[5];
	EventChar(uint32_t unicode) : unicode(unicode) {
		libv::unicode_to_utf8(utf8, unicode);
	}

	std::string toPrettyString() const {
		return fmt::format("Char: unicode = {}, utf8 = {}", unicode, utf8);
	}
};

struct EventCharMods {
	uint32_t unicode;
	int mods;
	char utf8[5];
	EventCharMods(uint32_t unicode, int mods) : unicode(unicode), mods(mods) {
		libv::unicode_to_utf8(utf8, unicode);
	}

	std::string toPrettyString() const {
		return fmt::format("Char Mods: unicode = {}, utf8 = {}, mode = {}", unicode, utf8, mods);
	}
};

struct EventDrop {
	std::vector<std::string> strings;
	EventDrop(int count, const char** path) : strings(count) {
		for (int i = 0; i < count; i++)
			strings.emplace_back(path[i]);
	}
	EventDrop(std::vector<std::string> strings) : strings(strings) { }

	std::string toPrettyString() const {
		return fmt::format("Drop: size = {}, contents: \n\"{}\"",
				strings.size(), boost::algorithm::join(strings, "\"\n\""));
	}
};

struct EventFramebufferSize {
	vec2i size;
	EventFramebufferSize(int width, int height) : size(width, height) { }
	EventFramebufferSize(vec2i size) : size(size) { }

	std::string toPrettyString() const {
		return fmt::format("Framebuffer Size: size = ({}, {})", size.x, size.y);
	}
};

struct EventKey {
	Key key;
	int scancode, action, mode;
	// TODO P4: Strongly typed enums for these variables, action?, mode?
	EventKey(int key, int scancode, int action, int mode) :
		key(static_cast<Key>(key)), scancode(scancode), action(action), mode(mode) { }

	std::string toPrettyString() const {
		return fmt::format("Key: key = {}, scancode = {}, action = {}, mode = {}",
				to_value(key), scancode, action, mode);
	}
};

struct EventMonitor {
	const Monitor& monitor;
	int event;
	EventMonitor(const Monitor& monitor, int event) : monitor(monitor), event(event) { }

	std::string toPrettyString() const {
//		return fmt::format("Monitor: name = {}, event = {}", monitor.name, event);
		return fmt::format("Monitor: event = {}", event);
	}
};

struct EventMouseButton {
	Mouse button;
	int action, mods;
	EventMouseButton(int button, int action, int mods) :
		button(static_cast<Mouse>(button)), action(action), mods(mods) { }

	std::string toPrettyString() const {
		return fmt::format("Mouse Button: button = {}, action = {}, mods = {}", to_value(button), action, mods);
	}
};

struct EventMouseEnter {
	int entered;
	EventMouseEnter(int entered) : entered(entered) { }

	std::string toPrettyString() const {
		return fmt::format("Mouse Enter: entered = {}", entered);
	}
};

struct EventMousePosition {
	vec2d position;
	EventMousePosition(double xpos, double ypos) : position(xpos, ypos) { }
	EventMousePosition(vec2d position) : position(position) { }

	std::string toPrettyString() const {
		return fmt::format("Mouse Position: position = ({}, {})", position.x, position.y);
	}
};

struct EventMouseScroll {
	vec2d offset;
	EventMouseScroll(double xoffset, double yoffset) : offset(xoffset, yoffset) { }
	EventMouseScroll(vec2d offset) : offset(offset) { }

	std::string toPrettyString() const {
		return fmt::format("Mouse Scroll: offset = ({}, {})", offset.x, offset.y);
	}
};

struct EventWindowFocus {
	int focused;
	EventWindowFocus(int focused) : focused(focused) { }

	std::string toPrettyString() const {
		return fmt::format("Window Focus: focused = {}", focused);
	}
};

struct EventWindowIconify {
	int iconified;
	EventWindowIconify(int iconified) : iconified(iconified) { }

	std::string toPrettyString() const {
		return fmt::format("Window Iconify: iconified = {}", iconified);
	}
};

struct EventWindowPosition {
	vec2i position;
	EventWindowPosition(int x, int y) : position(x, y) { }
	EventWindowPosition(vec2i position) : position(position) { }

	std::string toPrettyString() const {
		return fmt::format("Window Position: position = ({}, {})", position.x, position.y);
	}
};

struct EventWindowRefresh {
	std::string toPrettyString() const {
		return "Window Refresh";
	}
};

struct EventWindowSize {
	vec2i size;
	EventWindowSize(int x, int y) : size(x, y) { }
	EventWindowSize(vec2i size) : size(size) { }

	std::string toPrettyString() const {
		return fmt::format("Window Size: size = ({}, {})", size.x, size.y);
	}
};

// -------------------------------------------------------------------------------------------------

struct EventCloseRequest {
private:
	mutable bool abortClose = false;
public:
	void abort() const {
		abortClose = true;
	}
	bool isAborted() const {
		return abortClose;
	}
	std::string toPrettyString() const {
		return "Close Request";
	}
};

// -------------------------------------------------------------------------------------------------

struct EventContextInitialization {
	std::string toPrettyString() const {
		return "Context Initialization";
	}
};

struct EventContextRefresh {
	std::string toPrettyString() const {
		return "Context Refresh";
	}
};

struct EventContextTerminate {
	std::string toPrettyString() const {
		return "Context Terminate";
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace frame
} // namespace libv