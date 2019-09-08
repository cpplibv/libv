// File: Events.hpp, Created on 2013. december 31. 21:52, Author: Vader

#pragma once

// libv
#include <libv/math/vec.hpp>
#include <libv/input/input.hpp>
#include <libv/input/event.hpp>
// std
#include <array>
#include <string>
#include <variant>
#include <vector>


namespace libv {
namespace frame {

// -------------------------------------------------------------------------------------------------

class Monitor;

struct EventContentScale {
	libv::vec2f scale;

	inline EventContentScale(float xpos, float ypos) :
		scale(xpos, ypos) { }
	inline EventContentScale(libv::vec2f scale) :
		scale(scale) { }

	std::string toPrettyString() const;
};

struct EventFocus {
	bool focused;

	inline EventFocus(int focused) :
		focused(focused != 0) { }

	std::string toPrettyString() const;
};

struct EventFramebufferSize {
	libv::vec2i size;

	inline EventFramebufferSize(int width, int height) :
		size(width, height) { }
	inline EventFramebufferSize(libv::vec2i size) :
		size(size) { }

	std::string toPrettyString() const;
};

struct EventMaximize {
	bool maximized;

	inline EventMaximize(int maximized) :
		maximized(maximized != 0) { }

	std::string toPrettyString() const;
};

struct EventMinimize {
	bool minimized;

	inline EventMinimize(int minimized) :
		minimized(minimized != 0) { }

	std::string toPrettyString() const;
};

struct EventMonitor {
	const Monitor& monitor;
	libv::input::MonitorEvent event;

	inline EventMonitor(const Monitor& monitor, int event) :
		monitor(monitor),
		event{event} { }

	std::string toPrettyString() const;
};

struct EventPosition {
	libv::vec2i position;

	inline EventPosition(int x, int y) :
		position(x, y) { }
	inline EventPosition(libv::vec2i position) :
		position(position) { }

	std::string toPrettyString() const;
};

struct EventRefresh {
	std::string toPrettyString() const;
};

struct EventSize {
	libv::vec2i size;

	inline EventSize(int x, int y) :
		size(x, y) { }
	inline EventSize(libv::vec2i size) :
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

struct EventContextCreate {
	std::string toPrettyString() const;
};

struct EventContextUpdate {
	std::string toPrettyString() const;
};

struct EventContextDestroy {
	std::string toPrettyString() const;
};

// -------------------------------------------------------------------------------------------------

using Event = std::variant<
		libv::input::EventChar,
		EventContentScale,
		libv::input::EventDrop,
		EventFocus,
		EventFramebufferSize,
		libv::input::EventKey,
		EventMaximize,
		EventMinimize,
		libv::input::EventMouseButton,
		libv::input::EventMouseEnter,
		libv::input::EventMousePosition,
		libv::input::EventMouseScroll,
		EventPosition,
		EventRefresh,
		EventSize
>;

// -------------------------------------------------------------------------------------------------

} // namespace frame
} // namespace libv
