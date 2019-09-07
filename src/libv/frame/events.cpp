// File: events.hpp, Created on 2019. 02. 04. 23:42, Author: Vader

// hpp
#include <libv/frame/events.hpp>
// libv
#include <libv/utility/enum.hpp>
#include <libv/utility/utf8.hpp>
// ext
#include <boost/algorithm/string/join.hpp>
#include <fmt/format.h>
// pro
#include <libv/frame/monitor.hpp>


namespace libv {
namespace frame {

// -------------------------------------------------------------------------------------------------

EventChar::EventChar(uint32_t unicode) :
	unicode(unicode) {
	libv::unicode_to_utf8(unicode, utf8);
}

std::string EventChar::toPrettyString() const {
	return fmt::format("Char: unicode = {}, utf8 = {}", unicode, utf8);
}

std::string EventContentScale::toPrettyString() const {
	return fmt::format("Content Scale: scale = ({}, {})", scale.x, scale.y);
}

std::string EventDrop::toPrettyString() const {
	return fmt::format("Drop: size = {}, contents: \n\"{}\"",
			strings.size(), boost::algorithm::join(strings, "\"\n\""));
}

std::string EventFocus::toPrettyString() const {
	return fmt::format("Focus: focused = {}", focused);
}

std::string EventFramebufferSize::toPrettyString() const {
	return fmt::format("Framebuffer Size: size = ({}, {})", size.x, size.y);
}

std::string EventKey::toPrettyString() const {
	return fmt::format("Key: key = {} ({}), scancode = {}, action = {} ({}), mode = {}",
			libv::input::to_string(key), libv::to_value(key), scancode, libv::input::to_string(action), libv::to_value(action), libv::to_value(mods));
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

std::string EventMouseButton::toPrettyString() const {
	return fmt::format("Mouse Button: button = {} ({}), action = {} ({}), mods = {}",
			libv::input::to_string(button), libv::to_value(button), libv::input::to_string(action), libv::to_value(action), libv::to_value(mods));
}

std::string EventMouseEnter::toPrettyString() const {
	return fmt::format("Mouse Enter: entered = {}", entered);
}

std::string EventMousePosition::toPrettyString() const {
	return fmt::format("Mouse Position: position = ({}, {})", position.x, position.y);
}

std::string EventMouseScroll::toPrettyString() const {
	return fmt::format("Mouse Scroll: offset = ({}, {})", offset.x, offset.y);
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
