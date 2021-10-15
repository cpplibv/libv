// Project: libv.sys, File: src/libv/sys/clipboard.cpp

// hpp
#include <libv/sys/clipboard.hpp>
// ext
#include <clip/clip.h>
// pro
#include <libv/sys/log.hpp>


namespace libv {
namespace sys {

// -------------------------------------------------------------------------------------------------

std::string clipboard_text() {
	std::string result;
	bool success = clip::get_text(result);
	log_sys.error_if(!success, "Failed to get clipboard text");

	// NOTE: Cleanup any crlf line ending by removing every cr part
	std::erase(result, '\r');

	return result;
}

void clipboard_text(const std::string& string) {
	bool success = clip::set_text(string);
	log_sys.error_if(!success, "Failed to set clipboard text of length {}: [{}]", string.size(), string);
}

// -------------------------------------------------------------------------------------------------

} // namespace sys
} // namespace libv
