// Project: libv.res, File: src/libv/res/resource_path.hpp

#pragma once

#include <cassert>
#include <expected>
#include <span>
#include <string_view>
#include <utility>


namespace libv {
namespace res {

// -------------------------------------------------------------------------------------------------

// TODO P5: A better naming would be something ACL? related

void init_resource_mapping(std::span<std::pair<std::string_view, std::string_view>> virtual_path);

enum class ResourcePathEC {
	unlisted,
	unlisted_but_exists,
};

[[nodiscard]] constexpr inline std::string_view to_message(ResourcePathEC value) noexcept {
	switch (value) {
	case ResourcePathEC::unlisted: return "File unlisted";
	case ResourcePathEC::unlisted_but_exists: return "File exists but unlisted";
	}
	assert(false && "Invalid ResourcePathEC enum value");
	return "<<invalid-ResourcePathEC-enum-value>>";
}

[[nodiscard]] std::expected<std::string_view, ResourcePathEC> resource_path_ec(std::string_view virtual_path) noexcept;

/// @throws std::runtime_error if virtual_path was not found in the mapping rules
[[nodiscard]] std::string_view resource_path_or_throw(std::string_view virtual_path);

/// @throws std::runtime_error if virtual_path was not found in the mapping rules
// TODO P4: Remove this overload
[[nodiscard]] std::string_view resource_path(std::string_view virtual_path);

// -------------------------------------------------------------------------------------------------

} // namespace res
} // namespace libv
