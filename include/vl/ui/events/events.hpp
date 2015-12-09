// File: Events.hpp, Created on 2013. december 31. 21:52, Author: Vader

#pragma once

// vl
#include <vl/string.hpp>
#include <vl/vec.hpp>
// std
#include <cstring>
#include <string>
#include <vector>

namespace vl {
namespace ui {

class Monitor;
class Frame;

struct EventChar {
	unsigned int unicode;
	char utf8[5];
	EventChar(unsigned int unicode) : unicode(unicode) {
		vl::unicode_to_utf8(utf8, unicode);
	}
	EventChar(const EventChar& orig) = default;
};

struct EventCharMods {
	unsigned int unicode;
	int mods;
	char utf8[5];
	EventCharMods(unsigned int unicode, int mods) : unicode(unicode), mods(mods) {
		vl::unicode_to_utf8(utf8, unicode);
	}
	EventCharMods(const EventCharMods& orig) = default;
};

struct EventCursorEnter {
	int entered;
	EventCursorEnter(int entered) : entered(entered) { }
	EventCursorEnter(const EventCursorEnter& orig) = default;
};

struct EventCursorPos {
	double xpos, ypos;
	EventCursorPos(double xpos, double ypos) : xpos(xpos), ypos(ypos) { }
	EventCursorPos(const EventCursorPos& orig) = default;
};

struct EventDrop {
	std::vector<std::string> strings;
	EventDrop(int count, const char** path) : strings(count) {
		for (int i = 0; i < count; i++)
			strings.emplace_back(path[i]);
	}
	EventDrop(std::vector<std::string> strings) : strings(strings) { }
	EventDrop(const EventDrop& orig) = default;
};

struct EventFramebufferSize {
	ivec2 size;
	EventFramebufferSize(int width, int height) : size(width, height) { }
	EventFramebufferSize(const ivec2& size) : size(size) { }
	EventFramebufferSize(const EventFramebufferSize& orig) = default;
};

struct EventKey {
	int key, scancode, action, mode;
	// TODO P4: Strongly typed enums for these variables, action?, mode?
	EventKey(int key, int scancode, int action, int mode) :
		key(key), scancode(scancode), action(action), mode(mode) { }
	EventKey(const EventKey& orig) = default;

};

struct EventMonitor {
	const Monitor* monitor;
	int event;
	EventMonitor(const Monitor* monitor, int event) : monitor(monitor), event(event) { }
	EventMonitor(const EventMonitor& orig) = default;
};

struct EventMouseButton {
	int button, action, mods;
	EventMouseButton(int button, int action, int mods) :
		button(button), action(action), mods(mods) { }
	EventMouseButton(const EventMouseButton& orig) = default;
};

struct EventScroll {
	double xoffset, yoffset;
	EventScroll(double xoffset, double yoffset) : xoffset(xoffset), yoffset(yoffset) { }
	EventScroll(const EventScroll& orig) = default;
};

struct EventWindowClose {
};

struct EventWindowFocus {
	int focused;
	EventWindowFocus(int focused) : focused(focused) { }
	EventWindowFocus(const EventWindowFocus& orig) = default;
};

struct EventWindowIconify {
	int iconified;
	EventWindowIconify(int iconified) : iconified(iconified) { }
	EventWindowIconify(const EventWindowIconify& orig) = default;
};

struct EventWindowPos {
	ivec2 position;
	EventWindowPos(int x, int y) : position(x, y) { }
	EventWindowPos(const ivec2& position) : position(position) { }
	EventWindowPos(const EventWindowPos& orig) = default;
};

struct EventWindowRefresh {
};

struct EventWindowSize {
	ivec2 size;
	EventWindowSize(int x, int y) : size(x, y) { }
	EventWindowSize(const ivec2& size) : size(size) { }
	EventWindowSize(const EventWindowSize& orig) = default;
};

} //namespace ui
} //namespace vl