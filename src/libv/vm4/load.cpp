// File:   ui.cpp Author: Vader Created on 2019. február 14., 4:55

// hpp
#include <libv/vm4/load.hpp>
// libv
#include <libv/serialization/archive/binary.hpp>
//#include <libv/serialization/archive/json.hpp>
#include <libv/serialization/serialization.hpp>
// std
#include <sstream>
// pro
#include <libv/vm4/serialization/model.hpp>


namespace libv {
namespace vm4 {

// -------------------------------------------------------------------------------------------------

//Model load_or_throw(libv::span<std::byte> data) {
Model load_or_throw(const std::string& data) {
	// C++20: std::ispanstream ss(data, std::ios::in | std::ios::binary);
	std::istringstream ss(data, std::ios::in | std::ios::binary);
	libv::archive::BinaryInput ar(ss);

	Model model;
	ar(LIBV_NVP_NAMED("model", model));
	return model;
}

//std::optional<Model> load_optional(libv::span<std::byte> data) {
//	(void) data;
//	// Not implemented yet.
//	return std::nullopt;
//}

// -------------------------------------------------------------------------------------------------

} // namespace vm4
} // namespace libv
