// Project: libv.container, File: src/libv/container/vector_2d.hpp

#pragma once

// libv
#include <libv/math/vec.hpp>
// std
#include <cassert>
#include <memory>
#include <span>


namespace libv {

// -------------------------------------------------------------------------------------------------

template <typename T>
class sliding_window_2D {
private:
	libv::vec2i offset_{0, 0};
	libv::vec2i size_{0, 0};
	std::unique_ptr<T[]> storage;

public:
	constexpr inline sliding_window_2D() noexcept = default;
	constexpr inline sliding_window_2D(const sliding_window_2D&) = default;
	constexpr inline sliding_window_2D(sliding_window_2D&&) noexcept = default;
	constexpr inline sliding_window_2D& operator=(const sliding_window_2D&) & = default;
	constexpr inline sliding_window_2D& operator=(sliding_window_2D&&) & noexcept = default;

	constexpr inline sliding_window_2D(libv::vec2i offset, libv::vec2i size) :
			sliding_window_2D(offset.x, offset.y, size.x, size.y) {}

	constexpr inline sliding_window_2D(libv::vec2i offset, libv::vec2i size, const T& init_value) :
			sliding_window_2D(offset.x, offset.y, size.x, size.y, init_value) {}

	constexpr inline sliding_window_2D(int32_t offset_x_, int32_t offset_y_, int32_t size_x_, int32_t size_y_) :
			offset_(offset_x_, offset_y_),
			size_(size_x_, size_y_),
			storage(std::make_unique_for_overwrite<T[]>(size_x_ * size_y_)) {}

	constexpr inline sliding_window_2D(int32_t offset_x_, int32_t offset_y_, int32_t size_x_, int32_t size_y_, const T& init_value) :
			offset_(offset_x_, offset_y_),
			size_(size_x_, size_y_),
			storage(std::make_unique_for_overwrite<T[]>(size_x_ * size_y_)) {
		fill(init_value);
	}

public:
//	constexpr inline void slide(libv::vec2i xy) noexcept {
//		slide(xy.x, xy.y);
//	}
//
//	constexpr inline void slide(int32_t x, int32_t y) noexcept {
//		offset_.x += x;
//		offset_.y += y;
//	}

private:
	[[nodiscard]] constexpr inline int32_t modulo_positive(int32_t value, int32_t mod) const noexcept {
		const auto x = value % mod;
		return x < 0 ? x + mod : x;
	}

public:
	template <typename F>
	constexpr inline void foreach_wrap(int32_t start, int32_t count, int32_t wrap_size, const F& func) {
		if (count > 0)
			start += wrap_size;

		auto low = start;
		auto high = start + count;
		if (low > high)
			std::swap(low, high);

		for (int32_t i = low; i < high; ++i)
			// TODO P5: Optimize modulo_positive call to 2 for loop
			func(modulo_positive(i, wrap_size), i);
	}

	template <typename F>
	constexpr inline void foreach_wrap_opposite(int32_t start, int32_t count, int32_t wrap_size, const F& func) {
		if (count >= 0) {
			auto lead_index = modulo_positive(start, wrap_size);
			auto lead_pos = start;

			for (int32_t i = count; i < wrap_size; ++i) {
				if (lead_index + i >= wrap_size)
					lead_index -= wrap_size;

				func(lead_index + i, lead_pos + i);
			}
		} else {
			auto lead_index = modulo_positive(start, wrap_size);
			auto lead_pos = start;

			for (int32_t i = 0; i < wrap_size + count; ++i) {
				if (lead_index + i >= wrap_size)
					lead_index -= wrap_size;

				func(lead_index + i, lead_pos + i);
			}
		}
	}

public:
	template <typename Fn>
	constexpr inline void slide(int32_t slide_x, int32_t slide_y, const Fn& fn) noexcept {
		if (std::abs(slide_x) > size_.x) {
			// Transform multiple page jump on X to adjust offset and 1 page slide
			if (slide_x > 0) {
				offset_.x += slide_x - size_.x;
				slide_x = size_.x;
			} else {
				offset_.x += size_.x + slide_x;
				slide_x = -size_.x;
			}
		}
		if (std::abs(slide_y) > size_.y) {
			// Transform multiple page jump on Y to adjust offset and 1 page slide
			if (slide_y > 0) {
				offset_.y += slide_y - size_.y;
				slide_y = size_.y;
			} else {
				offset_.y += size_.y + slide_y;
				slide_y = -size_.y;
			}
		}

		// Full row slides
		foreach_wrap(offset_.y, slide_y, size_.y, [&](int32_t y, int32_t pos_y) {
			for (int32_t xi = 0; xi < size_.x; ++xi) {
				const auto pos_x = offset_.x + xi + slide_x;
				// TODO P5: Optimize modulo_positive call to 2 for loop
				const auto x = modulo_positive(pos_x, size_.x);
				const auto position = libv::vec2i{pos_x, pos_y};
				auto& element = storage[size_.x * y + x];
				element = fn(position, element);
			}
		});

		// Rest of the column slides
		if (slide_x != 0)
			foreach_wrap_opposite(offset_.y, slide_y, size_.y, [&](int32_t y, int32_t pos_y) {
				foreach_wrap(offset_.x, slide_x, size_.x, [&](int32_t x, int32_t pos_x) {
					const auto position = libv::vec2i{pos_x, pos_y};
					auto& element = storage[size_.x * y + x];
					element = fn(position, element);
				});
			});

		offset_.x += slide_x;
		offset_.y += slide_y;
	}

	template <typename Fn>
	constexpr inline void slide(libv::vec2i xy, const Fn& fn) noexcept {
		slide(xy.x, xy.y, fn);
	}

public:
	[[nodiscard]] constexpr inline bool contains(int32_t x, int32_t y) const noexcept {
		return
				offset_.x <= x && x < offset_.x + size_.x &&
				offset_.y <= y && y < offset_.y + size_.y;
	}

	[[nodiscard]] constexpr inline bool contains(libv::vec2i xy) const noexcept {
		return contains(xy.x, xy.y);
	}

	[[nodiscard]] constexpr inline libv::vec2i index_of(libv::vec2i xy) const noexcept {
		return index_of(xy.x, xy.y);
	}

	[[nodiscard]] constexpr inline libv::vec2i index_of(int32_t x, int32_t y) const noexcept {
		assert(contains(x, y));
		return {modulo_positive(x, size_.x), modulo_positive(y, size_.y)};

	}

	[[nodiscard]] constexpr inline int32_t index_of_raw(libv::vec2i xy) const noexcept {
		return index_of_raw(xy.x, xy.y);
	}

	[[nodiscard]] constexpr inline int32_t index_of_raw(int32_t x, int32_t y) const noexcept {
		const auto index = index_of(x, y);
		return size_.x * index.y + index.x;
	}

public:
	[[nodiscard]] constexpr inline T& operator()(libv::vec2i xy) noexcept {
		assert(contains(xy.x, xy.y));
		return storage[index_of_raw(xy.x, xy.y)];
	}

	[[nodiscard]] constexpr inline T& operator()(int32_t x, int32_t y) noexcept {
		assert(contains(x, y));
		return storage[index_of_raw(x, y)];
	}

	[[nodiscard]] constexpr inline const T& operator()(libv::vec2i xy) const noexcept {
		assert(contains(xy.x, xy.y));
		return storage[index_of_raw(xy.x, xy.y)];
	}

	[[nodiscard]] constexpr inline const T& operator()(int32_t x, int32_t y) const noexcept {
		assert(contains(x, y));
		return storage[index_of_raw(x, y)];
	}

public:
	[[nodiscard]] constexpr inline libv::vec2i storage_offset() const noexcept {
		return {modulo_positive(offset_.x, size_.x), modulo_positive(offset_.y, size_.y)};
	}

	[[nodiscard]] constexpr inline T& storage_at(int32_t x, int32_t y) noexcept {
		assert(0 <= x && x < size_.x);
		assert(0 <= y && y < size_.y);
		return storage[size_.x * y + x];
	}

	[[nodiscard]] constexpr inline const T& storage_at(int32_t x, int32_t y) const noexcept {
		assert(0 <= x && x < size_.x);
		assert(0 <= y && y < size_.y);
		return storage[size_.x * y + x];
	}

public:
	constexpr inline void fill(const T& value) {
		for (int32_t i = 0; i < size_.x * size_.y; ++i)
			storage[i] = value;
	}

public:
	[[nodiscard]] constexpr inline libv::vec2i size() const noexcept {
		return size_;
	}

	[[nodiscard]] constexpr inline int32_t size_x() const noexcept {
		return size_.x;
	}

	[[nodiscard]] constexpr inline int32_t size_y() const noexcept {
		return size_.y;
	}

	[[nodiscard]] constexpr inline libv::vec2i offset() const noexcept {
		return offset_;
	}

	[[nodiscard]] constexpr inline int32_t offset_x() const noexcept {
		return offset_.x;
	}

	[[nodiscard]] constexpr inline int32_t offset_y() const noexcept {
		return offset_.y;
	}

public:
	[[nodiscard]] constexpr inline T* data() noexcept {
		return storage.get();
	}

	[[nodiscard]] constexpr inline const T* data() const noexcept {
		return storage.get();
	}

public:
	[[nodiscard]] constexpr inline std::span<T> unordered_span() noexcept {
		return {storage.get(), static_cast<std::size_t>(size_.x * size_.y)};
	}

	[[nodiscard]] constexpr inline std::span<const T> unordered_span() const noexcept {
		return {storage.get(), static_cast<std::size_t>(size_.x * size_.y)};
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace libv
