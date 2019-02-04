// File: Events.hpp, Created on 2013. december 31. 21:52, Author: Vader

#pragma once

// libv
#include <libv/math/vec.hpp>
// std
#include <string>
#include <vector>
// pro
#include <libv/frame/inputs.hpp>


// TODO P5: Most of the event should get a observer_ref<Frame> as member, or even better: Frame&!
// TODO P5: Remove? EventWindowRefresh and EventWindowClose as they are handled by frame
//			EventWindowRefresh is not that simple, i think i have to work with it to 'force' refresh
//			if context is frozen due to event queue 'lock' in glfw (moving window)

namespace libv {
namespace frame {

// -------------------------------------------------------------------------------------------------

class Monitor;
class Frame;

struct EventChar {
	uint32_t unicode;
	char utf8[5]; /// Null terminated utf8 representation of the unicode character

	EventChar(uint32_t unicode);
	std::string toPrettyString() const;
};

struct EventCharMods {
	uint32_t unicode;
	char utf8[5];
	KeyModifier mods;

	EventCharMods(uint32_t unicode, int mods);
	std::string toPrettyString() const;
};

struct EventDrop {
	std::vector<std::string> strings;

	inline EventDrop(int count, const char** path) {
		strings.reserve(count);
		for (int i = 0; i < count; i++)
			strings.emplace_back(path[i]);
	}
	inline EventDrop(std::vector<std::string> strings) :
		strings(std::move(strings)) { }

	std::string toPrettyString() const;
};

struct EventFramebufferSize {
	vec2i size;

	inline EventFramebufferSize(int width, int height) :
		size(width, height) { }
	inline EventFramebufferSize(vec2i size) :
		size(size) { }

	std::string toPrettyString() const;
};

struct EventKey {
	Key key;
	int scancode;
	Action action;
	KeyModifier mods;

	// TODO P4: Strongly typed enums for these variables action
	inline EventKey(int key, int scancode, int action, int mods) :
		key{key},
		scancode(scancode),
		action{action},
		mods{mods} { }

	std::string toPrettyString() const;
};

struct EventMonitor {
	const Monitor& monitor;
	MonitorEvent event;

	inline EventMonitor(const Monitor& monitor, int event) :
		monitor(monitor),
		event{event} { }

	std::string toPrettyString() const;
};

struct EventMouseButton {
	Mouse button;
	Action action;
	KeyModifier mods;

	inline EventMouseButton(int button, int action, int mods) :
		button{button},
		action{action},
		mods{mods} { }

	std::string toPrettyString() const;
};

struct EventMouseEnter {
	bool entered;

	inline EventMouseEnter(int entered) :
		entered(entered != 0) { }

	std::string toPrettyString() const;
};

struct EventMousePosition {
	vec2d position;

	inline EventMousePosition(double xpos, double ypos) :
		position(xpos, ypos) { }
	inline EventMousePosition(vec2d position) :
		position(position) { }

	std::string toPrettyString() const;
};

struct EventMouseScroll {
	vec2d offset;

	inline EventMouseScroll(double xoffset, double yoffset) :
		offset(xoffset, yoffset) { }
	inline EventMouseScroll(vec2d offset) :
		offset(offset) { }

	std::string toPrettyString() const;
};

struct EventWindowFocus {
	bool focused;

	inline EventWindowFocus(int focused) :
		focused(focused != 0) { }

	std::string toPrettyString() const;
};

struct EventWindowIconify {
	bool iconified;

	inline EventWindowIconify(int iconified) :
		iconified(iconified != 0) { }

	std::string toPrettyString() const;
};

struct EventWindowPosition {
	vec2i position;

	inline EventWindowPosition(int x, int y) :
		position(x, y) { }
	inline EventWindowPosition(vec2i position) :
		position(position) { }

	std::string toPrettyString() const;
};

struct EventWindowRefresh {
	std::string toPrettyString() const;
};

struct EventWindowSize {
	vec2i size;

	inline EventWindowSize(int x, int y) :
		size(x, y) { }
	inline EventWindowSize(vec2i size) :
		size(size) { }

	std::string toPrettyString() const;
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
	std::string toPrettyString() const;
};

// -------------------------------------------------------------------------------------------------

struct EventContextInitialization {
	std::string toPrettyString() const;
};

struct EventContextRefresh {
	std::string toPrettyString() const;
};

struct EventContextTerminate {
	std::string toPrettyString() const;
};

// -------------------------------------------------------------------------------------------------

} // namespace frame
} // namespace libv
