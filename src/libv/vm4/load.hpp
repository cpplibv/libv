// Project: libv.vm4, File: src/libv/vm4/load.hpp

#pragma once

// std
#include <span>
#include <string_view>
//#include <optional>
// pro
#include <libv/vm4/fwd.hpp>
//#include <libv/vm4/model.hpp>


namespace libv {
namespace vm4 {

// -------------------------------------------------------------------------------------------------

//struct [[nodiscard]] LoadResult {
//	int error_code;
//	Model model;
//};

[[nodiscard]] Model load_or_throw(std::string_view data);
[[nodiscard]] Model load_or_throw(std::span<const std::byte> data);
//[[nodiscard]] std::optional<Model> load_optional(std::span<std::byte> data);

// -------------------------------------------------------------------------------------------------

} // namespace vm4
} // namespace libv
