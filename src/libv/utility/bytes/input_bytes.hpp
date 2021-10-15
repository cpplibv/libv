// Project: libv.utility, File: src/libv/utility/bytes/input_bytes.hpp

#pragma once

// std
#include <iosfwd>
#include <span>
#include <string_view>


namespace libv {

// -------------------------------------------------------------------------------------------------

// Some very rough tests to prove the performance of the idea
// https://quick-bench.com/q/LyXMRavpfzjZl_qP7adP0SGk79A

// -------------------------------------------------------------------------------------------------

class input_bytes {
	using read_fn_t = size_t(*)(void* object, std::byte* dst, size_t pos, size_t size) noexcept;

private:
	void* object;
	size_t input_offset;
	size_t input_size;
	read_fn_t read_fn;

public:
	explicit(false) input_bytes(const std::byte* data, size_t size) noexcept;
	explicit(false) input_bytes(const std::string_view s) noexcept;
	explicit(false) input_bytes(const std::span<const std::byte> s) noexcept;
	explicit(false) input_bytes(std::istream& s) noexcept;

	template <typename ContiguousRange>
		requires requires { std::span(std::declval<const ContiguousRange&>()); }
	explicit(false) inline input_bytes(const ContiguousRange& r) noexcept : input_bytes(std::as_bytes(std::span(r))) { }

public:
	/// Read \c size byte starting from \c pos into \c dst
	inline size_t read(std::byte* dst, size_t pos, size_t size) noexcept {
		// assert(input_size >= pos + size); // No need to assert here, as returned size marks and makes the error recoverable
		pos = input_offset + pos;
		return read_fn(object, dst, pos, std::min(size, input_size - pos));
	}

	[[nodiscard]] constexpr inline size_t size() const noexcept {
		return input_size;
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace libv
