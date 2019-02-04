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

EventCharMods::EventCharMods(uint32_t unicode, int mods) :
	unicode(unicode),
	mods{mods} {
	libv::unicode_to_utf8(unicode, utf8);
}

std::string EventCharMods::toPrettyString() const {
	return fmt::format("Char Mods: unicode = {}, utf8 = {}, mode = {}", unicode, utf8, to_value(mods));
}

std::string EventDrop::toPrettyString() const {
	return fmt::format("Drop: size = {}, contents: \n\"{}\"",
			strings.size(), boost::algorithm::join(strings, "\"\n\""));
}

std::string EventFramebufferSize::toPrettyString() const {
	return fmt::format("Framebuffer Size: size = ({}, {})", size.x, size.y);
}

std::string EventKey::toPrettyString() const {
	return fmt::format("Key: key = {}, scancode = {}, action = {}, mode = {}",
			to_value(key), scancode, to_value(action), to_value(mods));
}

std::string EventMonitor::toPrettyString() const {
	return fmt::format("Monitor: name = {}, event = {}", monitor.name, to_value(event));
}

std::string EventMouseButton::toPrettyString() const {
	return fmt::format("Mouse Button: button = {}, action = {}, mods = {}",
			to_value(button), to_value(action), to_value(mods));
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

std::string EventWindowFocus::toPrettyString() const {
	return fmt::format("Window Focus: focused = {}", focused);
}

std::string EventWindowIconify::toPrettyString() const {
	return fmt::format("Window Iconify: iconified = {}", iconified);
}

std::string EventWindowPosition::toPrettyString() const {
	return fmt::format("Window Position: position = ({}, {})", position.x, position.y);
}

std::string EventWindowRefresh::toPrettyString() const {
	return "Window Refresh";
}

std::string EventWindowSize::toPrettyString() const {
	return fmt::format("Window Size: size = ({}, {})", size.x, size.y);
}

// -------------------------------------------------------------------------------------------------

std::string EventCloseRequest::toPrettyString() const {
	return "Close Request";
}

// -------------------------------------------------------------------------------------------------

std::string EventContextInitialization::toPrettyString() const {
	return "Context Initialization";
}

std::string EventContextRefresh::toPrettyString() const {
	return "Context Refresh";
}

std::string EventContextTerminate::toPrettyString() const {
	return "Context Terminate";
}

// -------------------------------------------------------------------------------------------------

} // namespace frame
} // namespace libv
