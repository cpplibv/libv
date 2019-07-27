// Project: libv.control, Created on 2020.03.16. 16:41, Author: Császár Mátyás [Vader]

// hpp
#include <libv/control/input.hpp>
// pro
#include <libv/control/info.hpp>


namespace libv {
namespace control {

// -------------------------------------------------------------------------------------------------

std::ostream& Input::to_stream_symbol(std::ostream& os) const {
	return libv::control::to_stream_symbol(os, *this);
}
std::ostream& Input::to_stream_name(std::ostream& os) const{
	return libv::control::to_stream_name(os, *this);
}

std::string Input::to_string_symbol() const {
	return libv::control::to_string_symbol(*this);
}
std::string Input::to_string_name() const {
	return libv::control::to_string_name(*this);
}

std::string Input::to_string() const {
	return to_string_name();
}

// -------------------------------------------------------------------------------------------------

} // namespace control
} // namespace libv
