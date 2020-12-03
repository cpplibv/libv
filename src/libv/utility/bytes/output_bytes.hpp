// Project: libv.diff, File: src/libv/diff/diff.hpp, Author: Császár Mátyás [Vader]

#pragma once

// std
#include <iosfwd>
#include <string>
#include <vector>


namespace libv {

// -------------------------------------------------------------------------------------------------

// Some very rough test to prove the idea performance
// https://quick-bench.com/q/LyXMRavpfzjZl_qP7adP0SGk79A

// -------------------------------------------------------------------------------------------------

class output_bytes {
	using write_fn_t = void(*)(void* object, const std::byte* src, size_t pos, size_t size);

private:
	void* object;
	size_t offset;
	size_t size_;
	write_fn_t write_fn;

public:
	/*implicit*/ output_bytes(std::byte* data, size_t size) noexcept;
	/*implicit*/ output_bytes(std::string& s) noexcept;
	/*implicit*/ output_bytes(std::vector<std::byte>& s) noexcept;
	/*implicit*/ output_bytes(std::ostream& s) noexcept;

public:
	inline void write(const std::byte* src, size_t pos, size_t size) {
		write_fn(object, src, offset + pos, size);
	}

	[[nodiscard]] constexpr inline size_t size() const noexcept {
		return size_;
	}

	[[nodiscard]] constexpr inline bool bounded() const noexcept {
		return size_ != std::numeric_limits<std::size_t>::max();
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace libv
