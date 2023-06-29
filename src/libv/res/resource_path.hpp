//

#pragma once

// std
#include <span>
#include <string_view>


namespace libv {
namespace res {

// -------------------------------------------------------------------------------------------------

void init_resource_mapping(std::span<std::pair<std::string_view, std::string_view>> virtual_path);
[[nodiscard]] std::string_view resource_path(std::string_view virtual_path);

// -------------------------------------------------------------------------------------------------

} // namespace res
} // namespace libv
