// Project: libv.container, File: src/libv/container/vector_2d.hpp

#pragma once

// libv
#include <libv/math/vec.hpp>
#include <libv/meta/uninitialized.hpp>
// std
#include <memory>
#include <span>


namespace libv {

// -------------------------------------------------------------------------------------------------

template <typename T>
class vector_2D {
private:
	libv::vec2z size_;
	std::unique_ptr<T[]> storage;

public:
	constexpr inline vector_2D() noexcept = default;
	constexpr inline vector_2D(const vector_2D&) = default;
	constexpr inline vector_2D(vector_2D&&) noexcept = default;
	constexpr inline vector_2D& operator=(const vector_2D&) & = default;
	constexpr inline vector_2D& operator=(vector_2D&&) & noexcept = default;

	explicit constexpr inline vector_2D(libv::vec2z size) :
			vector_2D(size.x, size.y) {}

	constexpr inline vector_2D(libv::vec2z size, const T& init_value) :
			vector_2D(size.x, size.y, init_value) {}

	constexpr inline vector_2D(libv::vec2z size, libv::uninitialized_t tag) :
			vector_2D(size.x, size.y, tag) {}

	constexpr inline vector_2D(std::size_t x, std::size_t y) :
			size_(x, y),
			storage(std::make_unique_for_overwrite<T[]>(x * y)) {}

	constexpr inline vector_2D(std::size_t x, std::size_t y, const T& init_value) :
			size_(x, y),
			storage(std::make_unique_for_overwrite<T[]>(x * y)) {
		fill(init_value);
	}

	constexpr inline vector_2D(std::size_t x, std::size_t y, libv::uninitialized_t) :
			size_(x, y),
			storage(reinterpret_cast<T*>(::operator new[](sizeof(T) * x * y))) {

		// Raw allocation with placement new with libv::uninitialized which should be optimized away if supported
		if constexpr (requires { T(libv::uninitialized); })
			for (std::size_t i = 0; i < x * y; ++i)
				::new (&storage[i]) T(libv::uninitialized);
		else
			for (std::size_t i = 0; i < x * y; ++i)
				::new (&storage[i]) T();
	}

public:
	[[nodiscard]] constexpr inline T& operator()(libv::vec2z xy) noexcept {
		return storage[size_.x * xy.y + xy.x];
	}

	[[nodiscard]] constexpr inline T& operator()(std::size_t x, std::size_t y) noexcept {
		return storage[size_.x * y + x];
	}

	[[nodiscard]] constexpr inline const T& operator()(libv::vec2z xy) const noexcept {
		return storage[size_.x * xy.y + xy.x];
	}

	[[nodiscard]] constexpr inline const T& operator()(std::size_t x, std::size_t y) const noexcept {
		return storage[size_.x * y + x];
	}

public:
	constexpr inline void fill(const T& value) {
		for (std::size_t i = 0; i < size_.x * size_.y; ++i)
			storage[i] = value;
	}

public:
	[[nodiscard]] constexpr inline libv::vec2z size() const noexcept {
		return size_;
	}

	[[nodiscard]] constexpr inline std::size_t size_x() const noexcept {
		return size_.x;
	}

	[[nodiscard]] constexpr inline std::size_t size_y() const noexcept {
		return size_.y;
	}

	[[nodiscard]] constexpr inline T* data() noexcept {
		return storage.get();
	}

	[[nodiscard]] constexpr inline const T* data() const noexcept {
		return storage.get();
	}

	[[nodiscard]] constexpr inline std::span<T> span() noexcept {
		return {storage.get(), static_cast<std::size_t>(size_.x * size_.y)};
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace libv
