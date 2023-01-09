// Project: libv.sys, File: src/libv/sys/executable_path.hpp

#pragma once

// std
#include <filesystem>
#include <string>


namespace libv {
namespace sys {

// -------------------------------------------------------------------------------------------------

[[nodiscard]] std::string executable_path();
[[nodiscard]] std::filesystem::path executable_path_fs();

// -------------------------------------------------------------------------------------------------

} // namespace sys
} // namespace libv
