//

// hpp
#include <libv/res/resource_path.hpp>
// ext
#include <fmt/format.h>
// std
#include <stdexcept>
#include <unordered_map>


namespace libv {
namespace res {

// -------------------------------------------------------------------------------------------------

static std::unordered_map<std::string_view, std::string_view> mapping;
static bool enable_resource_mapping = false;

void init_resource_mapping(std::span<std::pair<std::string_view, std::string_view>> virtual_path) {
	mapping.insert(virtual_path.begin(), virtual_path.end());
	enable_resource_mapping = true;
}

std::string_view resource_path(std::string_view virtual_path) {
	if (!enable_resource_mapping)
		return virtual_path;

	const auto it = mapping.find(virtual_path);
	if (it != mapping.end())
		return it->second;

	throw std::runtime_error(fmt::format("Virtual path: \"{}\" is not found in resource mapping.", virtual_path));
}

// -------------------------------------------------------------------------------------------------

} // namespace res
} // namespace libv
