// File: events.hpp, Created on 2019. 02. 04. 23:42, Author: Vader

// hpp
#include <libv/input/event.hpp>
// libv
#include <libv/utility/enum.hpp>
#include <libv/utility/utf8.hpp>
// ext
#include <boost/algorithm/string/join.hpp>
#include <fmt/format.h>


namespace libv {
namespace input {

// -------------------------------------------------------------------------------------------------

EventChar::EventChar(uint32_t unicode) :
	unicode(unicode) {
	libv::unicode_to_utf8(unicode, utf8.data());
}

std::string EventChar::toPrettyString() const {
	return fmt::format("Char: unicode = {}, utf8 = {}", unicode, utf8.data());
}

std::string EventDrop::toPrettyString() const {
	return fmt::format("Drop: size = {}, contents: \n\"{}\"",
			strings.size(), boost::algorithm::join(strings, "\"\n\""));
}

std::string EventKey::toPrettyString() const {
	return fmt::format("Key: key = {} ({}), scancode = {}, action = {} ({}), mode = {}",
			libv::input::to_string(key), libv::to_value(key), scancode, libv::input::to_string(action), libv::to_value(action), libv::to_value(mods));
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

// -------------------------------------------------------------------------------------------------

} // namespace input
} // namespace libv