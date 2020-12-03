// Project: libv.diff, File: src/libv/diff/diff.hpp, Author: Császár Mátyás [Vader]

#pragma once

// std
#include <iosfwd>
#include <span>
#include <string_view>


namespace libv {

// -------------------------------------------------------------------------------------------------

// Some very rough test to prove the idea performance
// https://quick-bench.com/q/LyXMRavpfzjZl_qP7adP0SGk79A

// -------------------------------------------------------------------------------------------------

class input_bytes {
	using read_fn_t = size_t(*)(void* object, std::byte* dst, size_t pos, size_t size) noexcept;

private:
	void* object;
	size_t offset;
	size_t size_;
	read_fn_t read_fn;

public:
	/*implicit*/ input_bytes(const std::byte* data, size_t size) noexcept;
	/*implicit*/ input_bytes(const std::string_view s) noexcept;
	/*implicit*/ input_bytes(const std::span<const std::byte> s) noexcept;
	/*implicit*/ input_bytes(std::istream& s) noexcept;

public:
	inline size_t read(std::byte* dst, size_t pos, size_t size) noexcept {
		return read_fn(object, dst, offset + pos, size);
	}

	[[nodiscard]] constexpr inline size_t size() const noexcept {
		return size_;
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace libv
