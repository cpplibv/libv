// Project: libv.vm4, File: src/libv/vm4/load.cpp

// hpp
#include <libv/vm4/load.hpp>
// libv
#include <libv/serial/archive/binary.hpp>
//#include <libv/serial/archive/json.hpp>
#include <libv/serial/serial.hpp>
// std
#include <sstream>
// pro
#include <libv/vm4/model_serial.hpp>


namespace libv {
namespace vm4 {

// -------------------------------------------------------------------------------------------------

//Model load_or_throw(std::span<std::byte> data) {
Model load_or_throw(const std::string& data) {
	// C++20: std::ispanstream ss(data, std::ios::in | std::ios::binary);
	std::istringstream ss(data, std::ios::in | std::ios::binary);
	libv::archive::BinaryInput ar(ss);

	Model model;
	ar(LIBV_NVP_NAMED("model", model));
	return model;
}

//std::optional<Model> load_optional(std::span<std::byte> data) {
//	(void) data;
//	// Not implemented yet.
//	return std::nullopt;
//}

// -------------------------------------------------------------------------------------------------

} // namespace vm4
} // namespace libv
