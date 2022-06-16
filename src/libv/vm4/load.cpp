// Project: libv.vm4, File: src/libv/vm4/load.cpp

// hpp
#include <libv/vm4/load.hpp>
// libv
#include <libv/serial/archive/binary.hpp>
#include <libv/serial/serial.hpp>
// std
#include <spanstream>
// pro
#include <libv/vm4/model.hpp>
#include <libv/vm4/model_serial.hpp>


namespace libv {
namespace vm4 {

// -------------------------------------------------------------------------------------------------

Model load_or_throw(std::string_view data) {
	return load_or_throw(std::as_bytes(std::span(data)));
}

Model load_or_throw(std::span<const std::byte> data) {
	// std::ispanstream's API is a bit awkward, have to use char* instead of const char* even though there won't be any write operation
	// Or just use the custom stream API of the archive
	using CT = std::ispanstream::char_type;
	const auto span = std::span<CT>(const_cast<CT*>(reinterpret_cast<const CT*>(data.data())), data.size());

	std::ispanstream ss(span, std::ios::in | std::ios::binary);
	libv::archive::BinaryInput ar(ss);

	Model model;
	ar(LIBV_NVP_FORCED("model", model));
	return model;
}

//std::optional<Model> load_optional(std::span<const std::byte> data) {
//	(void) data;
//	// Not implemented yet.
//	return std::nullopt;
//}

// -------------------------------------------------------------------------------------------------

} // namespace vm4
} // namespace libv
