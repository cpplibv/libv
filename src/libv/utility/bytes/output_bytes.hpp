// Project: libv.utility, File: src/libv/utility/bytes/output_bytes.hpp

#pragma once

// std
#include <iosfwd>
#include <limits>
#include <string>
#include <vector>


namespace libv {

// -------------------------------------------------------------------------------------------------

// Some very rough tests to prove the performance of the idea
// https://quick-bench.com/q/LyXMRavpfzjZl_qP7adP0SGk79A

// -------------------------------------------------------------------------------------------------

class output_bytes {
	using write_fn_t = void(*)(void* object, const std::byte* src, size_t pos, size_t size);

private:
	void* object;
	size_t output_offset;
	size_t output_size;
	write_fn_t write_fn;

public:
	explicit(false) output_bytes(std::byte* data, size_t size) noexcept;
	explicit(false) output_bytes(std::string& s) noexcept;
	explicit(false) output_bytes(std::vector<std::byte>& s) noexcept;
	explicit(false) output_bytes(std::ostream& s) noexcept;

public:
	/// Write \c size byte starting at \c pos from \c src
	inline void write(const std::byte* src, size_t pos, size_t size) {
		write_fn(object, src, output_offset + pos, size);
	}

	[[nodiscard]] constexpr inline size_t size() const noexcept {
		return output_size;
	}

	[[nodiscard]] constexpr inline bool bounded() const noexcept {
		return output_size != std::numeric_limits<std::size_t>::max();
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace libv
