// File: Events.hpp, Created on 2013. december 31. 21:52, Author: Vader

#pragma once

// libv
#include <libv/string.hpp>
#include <libv/vec.hpp>
// std
#include <cstring>
#include <string>
#include <vector>

// TODO P5: Most of the event should get a observer_ptr<Frame> as member
// TODO P5: Review every event and change int to bool or enum, and (double, double) to dvec2
// TODO P5: Remove? EventWindowRefresh and EventWindowClose as they are handled by frame

// namespace Key {
//enum key_t {
//	Unknown,
//	F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
//	A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
//	Num0, Num1, Num2, Num3, Num4, Num5, Num6, Num7, Num8, Num9,
//	LeftBracket, RightBracket, Semicolon, Comma, Period, Quote, Slash, Backslash, Tilde, Equals, Hyphen,
//	Escape, Control, Shift, Alt, Space, Enter, Backspace, Tab, PageUp, PageDown, End, Home, Insert, Delete, Pause,
//	Left, Right, Up, Down,
//	Numpad0, Numpad1, Numpad2, Numpad3, Numpad4, Numpad5, Numpad6, Numpad7, Numpad8, Numpad9,
//	Add, Subtract, Multiply, Divide
//};
//}

namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class Monitor;
class Frame;

struct EventChar {
	uint32_t unicode;
	char utf8[5];
	EventChar(uint32_t unicode) : unicode(unicode) {
		libv::unicode_to_utf8(utf8, unicode);
	}
	EventChar(const EventChar& orig) = default;
};

struct EventCharMods {
	uint32_t unicode;
	int mods;
	char utf8[5];
	EventCharMods(uint32_t unicode, int mods) : unicode(unicode), mods(mods) {
		libv::unicode_to_utf8(utf8, unicode);
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
	EventFramebufferSize(ivec2 size) : size(size) { }
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
	EventWindowPos(ivec2 position) : position(position) { }
	EventWindowPos(const EventWindowPos& orig) = default;
};

struct EventWindowRefresh {
};

struct EventWindowSize {
	ivec2 size;
	EventWindowSize(int x, int y) : size(x, y) { }
	EventWindowSize(ivec2 size) : size(size) { }
	EventWindowSize(const EventWindowSize& orig) = default;
};

} // namespace ui
} // namespace libv