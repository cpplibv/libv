// Project: libv.frame, File: src/libv/frame/events.hpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
#include <libv/math/vec.hpp>
#include <libv/input/input.hpp>
#include <libv/input/event.hpp>
// std
#include <string>


namespace libv {
namespace frame {

// -------------------------------------------------------------------------------------------------

class Monitor;

struct EventContentScale {
	libv::vec2f scale;

	inline EventContentScale(float xpos, float ypos) noexcept :
		scale(xpos, ypos) { }
	explicit inline EventContentScale(libv::vec2f scale) noexcept :
		scale(scale) { }

	[[nodiscard]] std::string toPrettyString() const;
};

struct EventFocus {
	bool focused;

	explicit inline EventFocus(int focused) noexcept :
		focused(focused != 0) { }

	[[nodiscard]] std::string toPrettyString() const;
};

struct EventFramebufferSize {
	libv::vec2i size;

	inline EventFramebufferSize(int width, int height) noexcept :
		size(width, height) { }
	explicit inline EventFramebufferSize(libv::vec2i size) noexcept :
		size(size) { }

	[[nodiscard]] std::string toPrettyString() const;
};

struct EventMaximize {
	bool maximized;

	explicit inline EventMaximize(int maximized) noexcept :
		maximized(maximized != 0) { }

	[[nodiscard]] std::string toPrettyString() const;
};

struct EventMinimize {
	bool minimized;

	explicit inline EventMinimize(int minimized) noexcept :
		minimized(minimized != 0) { }

	[[nodiscard]] std::string toPrettyString() const;
};

struct EventMonitor {
	const Monitor& monitor;
	libv::input::MonitorEvent event;

	inline EventMonitor(const Monitor& monitor, int event) noexcept :
		monitor(monitor),
		event{event} { }

	[[nodiscard]] std::string toPrettyString() const;
};

struct EventPosition {
	libv::vec2i position;

	inline EventPosition(int x, int y) noexcept :
		position(x, y) { }
	explicit inline EventPosition(libv::vec2i position) noexcept :
		position(position) { }

	[[nodiscard]] std::string toPrettyString() const;
};

struct EventRefresh {
	[[nodiscard]] std::string toPrettyString() const;
};

struct EventSize {
	libv::vec2i size;

	inline EventSize(int x, int y) noexcept :
		size(x, y) { }
	explicit inline EventSize(libv::vec2i size) noexcept :
		size(size) { }

	[[nodiscard]] std::string toPrettyString() const;
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
	[[nodiscard]] std::string toPrettyString() const;
};

// -------------------------------------------------------------------------------------------------

struct EventContextCreate {
	[[nodiscard]] std::string toPrettyString() const;
};

struct EventContextUpdate {
	[[nodiscard]] std::string toPrettyString() const;
};

struct EventContextDestroy {
	[[nodiscard]] std::string toPrettyString() const;
};

// -------------------------------------------------------------------------------------------------

} // namespace frame
} // namespace libv
