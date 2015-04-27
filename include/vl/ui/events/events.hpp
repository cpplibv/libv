// File: Events.hpp, Created on 2013. december 31. 21:52, Author: Vader

#pragma once

// vl
#include <vl/string.hpp>
#include <vl/vec.hpp>
// std
#include <string>
#include <vector>

namespace vl {
namespace ui {

class Monitor;
class Frame;

struct EventChar {
	unsigned int codepoint;
	std::string asUTF8() const {
		return unicode_to_utf8(codepoint);
	}
	EventChar(unsigned int codepoint) : codepoint(codepoint) { }
	EventChar(const EventChar& orig) : codepoint(orig.codepoint) { }
};

struct EventCharMods {
	unsigned int codepoint;
	int mods;
	std::string asUTF8() const {
		return unicode_to_utf8(codepoint);
	}
	EventCharMods(unsigned int codepoint, int mods) : codepoint(codepoint), mods(mods) { }
	EventCharMods(const EventCharMods& orig) : codepoint(orig.codepoint), mods(orig.mods) { }
};

struct EventCursorEnter {
	int entered;
	EventCursorEnter(int entered) : entered(entered) { }
	EventCursorEnter(const EventCursorEnter& orig) : entered(orig.entered) { }
};

struct EventCursorPos {
	double xpos, ypos;
	EventCursorPos(double xpos, double ypos) : xpos(xpos), ypos(ypos) { }
	EventCursorPos(const EventCursorPos& orig) : xpos(orig.xpos), ypos(orig.ypos) { }
};

struct EventDrop {
	std::vector<std::string> paths;
	EventDrop(int count, const char** path) : paths(count) {
		for (int i = 0; i < count; i++)
			paths.emplace_back(path[i]);
	}
	EventDrop(const EventDrop& orig) : paths(orig.paths) { }
};

struct EventFramebufferSize {
	int width, height;
	EventFramebufferSize(int width, int height) : width(width), height(height) { }
	EventFramebufferSize(const EventFramebufferSize& orig) : width(orig.width), height(orig.height) { }

};

struct EventKey {
	int key, scancode, action, mode;
	EventKey(int key, int scancode, int action, int mode) :
		key(key), scancode(scancode), action(action), mode(mode) { }
	EventKey(const EventKey& orig) :
		key(orig.key), scancode(orig.scancode), action(orig.action), mode(orig.mode) { }

};

struct EventMonitor {
	const Monitor* monitor;
	int event;
	EventMonitor(const Monitor* monitor, int event) : monitor(monitor), event(event) { }
	EventMonitor(const EventMonitor& orig) : monitor(orig.monitor), event(orig.event) { }
};

struct EventMouseButton {
	int button, action, mods;
	EventMouseButton(int button, int action, int mods) :
		button(button), action(action), mods(mods) { }
	EventMouseButton(const EventMouseButton& orig) :
		button(orig.button), action(orig.action), mods(orig.mods) { }
};

struct EventScroll {
	double xoffset, yoffset;
	EventScroll(double xoffset, double yoffset) : xoffset(xoffset), yoffset(yoffset) { }
	EventScroll(const EventScroll& orig) : xoffset(orig.xoffset), yoffset(orig.yoffset) { }
};

struct EventWindowClose {
};

struct EventWindowFocus {
	int focused;
	EventWindowFocus(int focused) : focused(focused) { }
	EventWindowFocus(const EventWindowFocus& orig) : focused(orig.focused) { }
};

struct EventWindowIconify {
	int iconified;
	EventWindowIconify(int iconified) : iconified(iconified) { }
	EventWindowIconify(const EventWindowIconify& orig) : iconified(orig.iconified) { }
};

struct EventWindowPos {
	ivec2 position;
	EventWindowPos(int x, int y) : position(x, y) { }
	EventWindowPos(const ivec2& position) : position(position) { }
	EventWindowPos(const EventWindowPos& orig) : position(orig.position) { }
};

struct EventWindowRefresh {
};

struct EventWindowSize {
	ivec2 size;
	EventWindowSize(int x, int y) : size(x, y) { }
	EventWindowSize(const ivec2& size) : size(size) { }
	EventWindowSize(const EventWindowSize& orig) : size(orig.size) { }
};

} //namespace ui
} //namespace vl