// Project: libv.diff, File: src/libv/diff/diff.hpp, Author: Császár Mátyás [Vader]

#pragma once

// std
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <vector>


namespace libv {
namespace diff {

// -------------------------------------------------------------------------------------------------

static constexpr size_t default_match_block_size = 64;

using bv = std::span<const std::byte>;
using sv = std::string_view;

struct diff_info {
private:
	static constexpr size_t invalid = std::numeric_limits<size_t>::max();

public:
	size_t old_size = invalid;
	size_t new_size = invalid;

public:
	[[nodiscard]] constexpr inline bool valid() const noexcept {
		return
				old_size != invalid &&
				new_size != invalid;
	}
	[[nodiscard]] explicit constexpr inline operator bool() const noexcept {
		return valid();
	}
	[[nodiscard]] constexpr inline bool operator!() const noexcept {
		return !valid();
	}
};

// -------------------------------------------------------------------------------------------------

/// Create a diff from \c old to \c new.
///
/// \param old - The old version of the data
/// \param new_ - The new version of the data
/// \param match_block_size - Smaller block size improves compression but at the cost of performance. Recommended 16-16384
/// \return The resulting diff that can be applied to \c old to get \c new
[[nodiscard]] std::string create_diff(sv old, sv new_, size_t match_block_size = default_match_block_size);
[[nodiscard]] std::vector<std::byte> create_diff(bv old, bv new_, size_t match_block_size = default_match_block_size);
void create_diff(sv old, sv new_, std::string& out_diff, size_t match_block_size = default_match_block_size);
void create_diff(bv old, bv new_, std::vector<std::byte>& out_diff, size_t match_block_size = default_match_block_size);
/// All streams has to be binary and seekable
void create_diff(std::istream& old, std::istream& new_, std::ostream& out_diff, size_t match_block_size = default_match_block_size);

/// Check if the \c diff applied to \c old will result in \c new.
///
/// \param old - The old version of the data
/// \param new_ - The new version of the data
/// \param diff - The diff that will be applied to \c old
/// \return Returns true if \c diff can be applied to \c old and it results in \c new
[[nodiscard]] bool check_diff(sv old, sv new_, sv diff);
[[nodiscard]] bool check_diff(bv old, bv new_, bv diff);
/// All streams has to be binary and seekable
[[nodiscard]] bool check_diff(std::istream& old, std::istream& new_, std::istream& diff);

/// Applies \c diff to \c old.
///
/// \param old - The old version of the data
/// \param diff - The diff that will be applied to \c old
/// \return Returns the new version of the data after applying the \c diff or an empty optional if the \c diff cannot be applied to \olc
[[nodiscard]] std::optional<std::string> apply_patch(sv old, sv diff);
[[nodiscard]] std::optional<std::vector<std::byte>> apply_patch(bv old, bv diff);
[[nodiscard]] bool apply_patch(sv old, sv diff, std::string& out_new);
[[nodiscard]] bool apply_patch(bv old, bv diff, std::vector<std::byte>& out_new);
/// All streams has to be binary and seekable
[[nodiscard]] bool apply_patch(std::istream& old, std::istream& diff, std::ostream& out_new);

/// \return Returns a non valid diff_info object upon failure, otherwise returns the diff info
[[nodiscard]] diff_info get_diff_info(sv diff);
[[nodiscard]] diff_info get_diff_info(bv diff);
/// All streams has to be binary and seekable
[[nodiscard]] diff_info get_diff_info(std::istream& diff);

// -------------------------------------------------------------------------------------------------

} // namespace diff
} // namespace libv
