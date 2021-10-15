// Project: libv.container, File: src/libv/container/vector_2d.hpp

#pragma once

// libv
#include <libv/math/vec.hpp>
// std
#include <vector>


namespace libv {

// -------------------------------------------------------------------------------------------------

template <typename T>
class vector_2D {
private:
	libv::vec2z size_;
	std::vector<T> storage;

public:
	constexpr inline vector_2D() = delete;
	constexpr inline vector_2D(const vector_2D&) = default;
	constexpr inline vector_2D(vector_2D&&) noexcept = default;
	constexpr inline vector_2D& operator=(const vector_2D&) & = default;
	constexpr inline vector_2D& operator=(vector_2D&&) & noexcept = default;

	explicit constexpr inline vector_2D(libv::vec2z size) :
			size_(size),
			storage(size.x * size.y) {}

	explicit constexpr inline vector_2D(libv::vec2z size, const T& init_value) :
			size_(size),
			storage(size.x * size.y, init_value) {}

	constexpr inline vector_2D(size_t x, size_t y) :
			size_(x, y),
			storage(x * y) {}

	constexpr inline vector_2D(size_t x, size_t y, const T& init_value) :
			size_(x, y),
			storage(x * y, init_value) {}

public:
	[[nodiscard]] constexpr inline T& operator()(libv::vec2z xy) noexcept {
		return storage[size_.x * xy.y + xy.x];
	}

	[[nodiscard]] constexpr inline T& operator()(size_t x, size_t y) noexcept {
		return storage[size_.x * y + x];
	}

	[[nodiscard]] constexpr inline const T& operator()(libv::vec2z xy) const noexcept {
		return storage[size_.x * xy.y + xy.x];
	}

	[[nodiscard]] constexpr inline const T& operator()(size_t x, size_t y) const noexcept {
		return storage[size_.x * y + x];
	}

public:
	[[nodiscard]] constexpr inline libv::vec2z size() const noexcept {
		return size_;
	}

	[[nodiscard]] constexpr inline size_t size_x() const noexcept {
		return size_.x;
	}

	[[nodiscard]] constexpr inline size_t size_y() const noexcept {
		return size_.y;
	}

public:
	[[nodiscard]] constexpr inline T* data() noexcept {
		return storage.data();
	}

	[[nodiscard]] constexpr inline const T* data() const noexcept {
		return storage.data();
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace libv
