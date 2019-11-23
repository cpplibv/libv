// File: Model.hpp, Created on 2015. január 17. 7:21, Author: Vader

#pragma once

// libv
#include <libv/utility/span.hpp>
// std
#include <optional>
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
//Model load_or_throw(libv::span<std::byte> data);
//std::optional<Model> load_optional(libv::span<std::byte> data);

// -------------------------------------------------------------------------------------------------

} // namespace vm4
} // namespace libv
