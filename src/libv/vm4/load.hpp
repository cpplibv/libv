// Project: libv.vm4, File: src/libv/vm4/load.hpp, Author: Császár Mátyás [Vader]

#pragma once

// std
#include <optional>
//#include <span>
// pro
#include <libv/vm4/model.hpp>


namespace libv {
namespace vm4 {

// -------------------------------------------------------------------------------------------------

//struct LoadResult {
//	int error_code;
//	Model model;
//};

Model load_or_throw(const std::string& data);
//Model load_or_throw(std::span<std::byte> data);
//std::optional<Model> load_optional(std::span<std::byte> data);

// -------------------------------------------------------------------------------------------------

} // namespace vm4
} // namespace libv
