// Project: libv.res, File: src/libv/res/resource_path.cpp

// hpp
#include <libv/res/resource_path.hpp>
// ext
#include <fmt/format.h>
// std
#include <cassert>
#include <filesystem>
#include <stdexcept>
#include <unordered_map>


namespace libv {
namespace res {

// -------------------------------------------------------------------------------------------------

namespace {
// Virtual file path -> Physical file path
std::unordered_map<std::string_view, std::string_view> mapping;
bool enable_resource_mapping = false;
} // namespace

void init_resource_mapping(std::span<std::pair<std::string_view, std::string_view>> virtual_path) {
	mapping.insert(virtual_path.begin(), virtual_path.end());
	enable_resource_mapping = true;
}

std::expected<std::string_view, ResourcePathEC> resource_path_ec(std::string_view virtual_path) noexcept {
	if (!enable_resource_mapping)
		return virtual_path;

	const auto it = mapping.find(virtual_path);
	if (it != mapping.end())
		return it->second;

	if (std::filesystem::exists(virtual_path))
		return std::unexpected(ResourcePathEC::unlisted_but_exists);

	return std::unexpected(ResourcePathEC::unlisted);
}

std::string_view resource_path_or_throw(std::string_view virtual_path) {
	if (!enable_resource_mapping)
		return virtual_path;

	const auto result = resource_path_ec(virtual_path);
	if (result)
		return result.value();

	switch (result.error()) {
	case ResourcePathEC::unlisted:
		throw std::runtime_error(fmt::format("Virtual path: \"{}\" is not found in resource mapping.", virtual_path));
	case ResourcePathEC::unlisted_but_exists:
		throw std::runtime_error(fmt::format("Virtual path: \"{}\" is not found in resource mapping,"
				" but the path points to an existing file. Possibly a missing resource mapping entry.",
				virtual_path));
	}

	assert(false && "Internal error: Invalid ResourcePathEC enum value");
	return "";
}

std::string_view resource_path(std::string_view virtual_path) {
	return resource_path_or_throw(virtual_path);
}

// -------------------------------------------------------------------------------------------------

} // namespace res
} // namespace libv
