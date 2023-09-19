// Project: libv.re, File: src/libv/re/node/proxy.cpp

#include <libv/re/node/proxy.hpp>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

void increase_ref_count(Proxy* ptr) {
	libv::ref_count_access::increase_ref_count(ptr);
}
void decrease_ref_count(Proxy* ptr) {
	libv::ref_count_access::decrease_ref_count(ptr);
}

// -------------------------------------------------------------------------------------------------

Proxy::~Proxy() {
	// For the sake of forward declared types
}

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
