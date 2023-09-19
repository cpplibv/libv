// Project: libv.sys, File: src/libv/sys/executable_path.cpp

// hpp
#include <libv/sys/executable_path.hpp>
// ext
#include <boost/dll/runtime_symbol_info.hpp>


namespace libv {
namespace sys {

// -------------------------------------------------------------------------------------------------

std::string executable_path() {
	boost::dll::fs::error_code ignore;
	return boost::dll::program_location(ignore).generic_string();
}

std::filesystem::path executable_path_fs() {
	return {executable_path()};
}

// -------------------------------------------------------------------------------------------------

} // namespace sys
} // namespace libv
