// Project: libv.utility, File: src/libv/utility/memory/dynamic_buffer.hpp

#pragma once

#include <libv/meta/uninitialized.hpp>

#include <cstddef>
#include <memory>


namespace libv {

// -------------------------------------------------------------------------------------------------

class dynamic_buffer {
private:
	std::unique_ptr<std::byte[]> data_;
	std::size_t size_ = 0;

public:
	constexpr inline dynamic_buffer() noexcept = default;
	constexpr inline explicit dynamic_buffer(std::size_t size) :
		data_(std::make_unique<std::byte[]>(size)), size_(size) { }

	constexpr inline explicit dynamic_buffer(std::size_t size, libv::uninitialized_t) :
		data_(std::make_unique_for_overwrite<std::byte[]>(size)), size_(size) { }

	//...

public:
	//... resize_with_copy
	//... resize_with_discard

public:
	constexpr inline std::byte* data() noexcept {
		return data_.get();
	}
	[[nodiscard]] constexpr inline const std::byte* data() const noexcept {
		return data_.get();
	}
	[[nodiscard]] constexpr inline std::size_t size() const noexcept {
		return size();
	}

	//...
};

// -------------------------------------------------------------------------------------------------

} // namespace libv
