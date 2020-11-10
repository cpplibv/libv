// Project: libv.diff, File: src/libv/diff/diff.hpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
#include <libv/utility/bytes/input_bytes.hpp>
#include <libv/utility/bytes/output_bytes.hpp>
// std
#include <optional>
#include <string>
#include <vector>


namespace libv {
namespace diff {

// -------------------------------------------------------------------------------------------------

static constexpr size_t default_match_block_size = 64;

// -------------------------------------------------------------------------------------------------

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

namespace detail { // ------------------------------------------------------------------------------

void aux_create_diff(libv::input_bytes old, libv::input_bytes new_, libv::output_bytes out_diff, size_t match_block_size);
[[nodiscard]] diff_info aux_get_diff_info(libv::input_bytes diff);
[[nodiscard]] bool aux_check_diff(libv::input_bytes old, libv::input_bytes new_, libv::input_bytes diff);
[[nodiscard]] bool apply_patch(libv::input_bytes old, libv::input_bytes diff, libv::output_bytes out_new);

} // namespace -------------------------------------------------------------------------------------

/// Create a diff from \c old to \c new and appends at the end of \c diff.
///
/// \param old - The old version of the data
/// \param new_ - The new version of the data
/// \param match_block_size - Smaller block size improves compression but at the cost of performance. Recommended 16-16384
/// \return The resulting diff that can be applied to \c old to get \c new
template <typename In0, typename In1, typename Out>
inline void create_diff(In0&& old, In1&& new_, Out&& out_diff, size_t match_block_size = default_match_block_size) {
	detail::aux_create_diff(libv::input_bytes(old), libv::input_bytes(new_), libv::output_bytes(out_diff), match_block_size);
}

/// Returns a diff created from \c old to \c new.
///
/// \template T - The output type used for diff (std::string or std::vector<std::byte>)
/// \param old - The old version of the data
/// \param new_ - The new version of the data
/// \param match_block_size - Smaller block size improves compression but at the cost of performance. Recommended 16-16384
/// \return The resulting diff that can be applied to \c old to get \c new
template <typename T, typename In0, typename In1>
[[nodiscard]] inline T create_diff(In0&& old, In1&& new_, size_t match_block_size = default_match_block_size) {
	T diff;
	create_diff(old, new_, diff, match_block_size);
	return diff;
}

template <typename In0, typename In1>
[[nodiscard]] inline std::string create_diff_str(In0&& old, In1&& new_, size_t match_block_size = default_match_block_size) {
	return create_diff<std::string>(old, new_, match_block_size);
}

template <typename In0, typename In1>
[[nodiscard]] inline std::vector<std::byte> create_diff_bin(In0&& old, In1&& new_, size_t match_block_size = default_match_block_size) {
	return create_diff<std::vector<std::byte>>(old, new_, match_block_size);
}

/// \return Returns a non valid diff_info object upon failure, otherwise returns the diff info
template <typename In0>
[[nodiscard]] diff_info get_diff_info(In0&& diff) {
	return detail::aux_get_diff_info(libv::input_bytes(diff));
}

/// Check if the \c diff applied to \c old will result in \c new.
///
/// \param old - The old version of the data
/// \param new_ - The new version of the data
/// \param diff - The diff that will be applied to \c old
/// \return Returns true if \c diff can be applied to \c old and it results in \c new
template <typename In0, typename In1, typename In2>
[[nodiscard]] inline bool check_diff(In0&& old, In1&& new_, In2&& diff) {
	return detail::aux_check_diff(libv::input_bytes(old), libv::input_bytes(new_), libv::input_bytes(diff));
}

/// Applies \c diff to \c old.
///
/// \param old - The old version of the data
/// \param diff - The diff that will be applied to \c old
/// \return Returns the new version of the data after applying the \c diff or an empty optional if the \c diff cannot be applied to \olc
template <typename In0, typename In1, typename Out>
[[nodiscard]] inline bool apply_patch(In0&& old, In1&& diff, Out&& out_new) {
	return detail::apply_patch(libv::input_bytes(old), libv::input_bytes(diff), libv::output_bytes(out_new));
}

/// Applies \c diff to \c old.
///
/// \param old - The old version of the data
/// \param diff - The diff that will be applied to \c old
/// \return Returns the new version of the data after applying the \c diff or an empty optional if the \c diff cannot be applied to \olc
template <typename T, typename In0, typename In1>
[[nodiscard]] inline std::optional<T> apply_patch(In0&& old, In1&& diff) {
	std::optional<T> new_(std::in_place);

	const auto success = apply_patch(old, diff, *new_);
	if (!success)
		new_.reset();

	return new_;
}

template <typename In0, typename In1>
[[nodiscard]] inline std::optional<std::string> apply_patch_str(In0&& old, In1&& diff) {
	return apply_patch<std::string>(old, diff);
}

template <typename In0, typename In1>
[[nodiscard]] inline std::optional<std::vector<std::byte>> apply_patch_bin(In0&& old, In1&& diff) {
	return apply_patch<std::vector<std::byte>>(old, diff);
}

// -------------------------------------------------------------------------------------------------

} // namespace diff
} // namespace libv
