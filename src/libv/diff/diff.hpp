// Project: libv.diff, File: src/libv/diff/diff.hpp, Author: Császár Mátyás [Vader]

#pragma once

// std
#include <string>
#include <string_view>


namespace libv {
namespace diff {

// -------------------------------------------------------------------------------------------------

/// @param old_
/// @param new_
/// @param match_block_size - Smaller block size improves compression but with a performance cost. Recommended 16-16384
/// @return The resulting diff that can be applied to \c old to get \c new
[[nodiscard]] std::string create_diff(std::string_view old_, std::string_view new_, size_t match_block_size = 64);


std::string check_patch();
std::string apply_patch();

// -------------------------------------------------------------------------------------------------

} // namespace diff
} // namespace libv
