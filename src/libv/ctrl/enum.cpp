// Project: libv.ctrl, File: src/libv/ctrl/enum.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <libv/ctrl/enum.hpp>


namespace libv {
namespace ctrl {

// -------------------------------------------------------------------------------------------------

std::string_view to_string(const DigitalInputAction& var) {
	switch (var) {
	case DigitalInputAction::auto_: return "auto";

	case DigitalInputAction::press: return "press";
	case DigitalInputAction::repeat: return "repeat";
	case DigitalInputAction::release: return "release";
	case DigitalInputAction::any: return "any";

	case DigitalInputAction::hold: return "hold";
	case DigitalInputAction::free: return "free";
	}

	return "<<invalid>>";
}

std::string_view to_string(const OperationBinary& var) {
	switch (var) {
	case OperationBinary::enable: return "enable";
	case OperationBinary::disable: return "disable";
	case OperationBinary::toggle: return "toggle";

	case OperationBinary::state: return "state";
	case OperationBinary::inverted: return "inverted";
	}

	return "<<invalid>>";
}

// -------------------------------------------------------------------------------------------------

} // namespace ctrl
} // namespace libv
