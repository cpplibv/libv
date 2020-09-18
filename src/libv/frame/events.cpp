// Project: libv.frame, File: src/libv/frame/events.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <libv/frame/events.hpp>
// libv
#include <libv/utility/enum.hpp>
// ext
#include <fmt/format.h>
// pro
#include <libv/frame/monitor.hpp>


namespace libv {
namespace frame {

// -------------------------------------------------------------------------------------------------

std::string EventContentScale::toPrettyString() const {
	return fmt::format("Content Scale: scale = ({}, {})", scale.x, scale.y);
}

std::string EventFocus::toPrettyString() const {
	return fmt::format("Focus: focused = {}", focused);
}

std::string EventFramebufferSize::toPrettyString() const {
	return fmt::format("Framebuffer Size: size = ({}, {})", size.x, size.y);
}

std::string EventMaximize::toPrettyString() const {
	return fmt::format("Window Maximize: maximized = {}", maximized);
}

std::string EventMinimize::toPrettyString() const {
	return fmt::format("Window Minimize: minimized = {}", minimized);
}

std::string EventMonitor::toPrettyString() const {
	return fmt::format("Monitor: name = {}, event = {} ({})", monitor.name, libv::input::to_string(event), libv::to_value(event));
}

std::string EventPosition::toPrettyString() const {
	return fmt::format("Window Position: position = ({}, {})", position.x, position.y);
}

std::string EventRefresh::toPrettyString() const {
	return "Window Refresh";
}

std::string EventSize::toPrettyString() const {
	return fmt::format("Window Size: size = ({}, {})", size.x, size.y);
}

// -------------------------------------------------------------------------------------------------

std::string EventCloseRequest::toPrettyString() const {
	return "Close Request";
}

// -------------------------------------------------------------------------------------------------

std::string EventContextCreate::toPrettyString() const {
	return "Context Initialization";
}

std::string EventContextUpdate::toPrettyString() const {
	return "Context Refresh";
}

std::string EventContextDestroy::toPrettyString() const {
	return "Context Terminate";
}

// -------------------------------------------------------------------------------------------------

} // namespace frame
} // namespace libv
