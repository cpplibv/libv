// Project: libv.ui, File: src/libv/ui/property/size.hpp

#pragma once

// libv
#include <libv/math/vec.hpp>
#include <libv/meta/force_inline.hpp>
#include <libv/utility/float_equal.hpp>
// std
#include <utility>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

struct SizeDim {
	float pixel = 0.0f;   /// Fixed size
	float percent = 0.0f; /// Parent size percentile
	float ratio = 1.0f;   /// Parent leftover space ratio
	bool dynamic = false; /// Yield all available space and shrink to the dynamic size

	constexpr LIBV_FORCE_INLINE SizeDim() noexcept = default;
	constexpr LIBV_FORCE_INLINE SizeDim(float pixel, float percent, float ratio, bool dynamic) noexcept :
		pixel(pixel), percent(percent), ratio(ratio), dynamic(dynamic) { }

//	static inline SizeDim add(const SizeDim& a, const SizeDim& b) {
//		return SizeDim{
//			a.pixel + b.pixel,
//			a.percent + b.percent,
//			a.ratio + b.ratio,
//			a.dynamic || b.dynamic
//		};
//	}
//	static inline SizeDim max(const SizeDim& a, const SizeDim& b) {
//		return SizeDim{
//			std::max(a.pixel, b.pixel),
//			std::max(a.percent, b.percent),
//			std::max(a.ratio, b.ratio),
//			a.dynamic || b.dynamic
//		};
//	}

	template <typename OStream>
	friend OStream& operator<<(OStream& os, const SizeDim& size) {
		if (std::not_equal_to<float>()(size.pixel, 0.0f))
			os << size.pixel << "px";
		if (std::not_equal_to<float>()(size.percent, 0.0f))
			os << size.percent << '%';
		if (std::not_equal_to<float>()(size.ratio, 0.0f))
			os << size.ratio << 'r';
		if (size.dynamic)
			os << "D";
		return os;
	}

	[[nodiscard]] constexpr inline bool operator==(const SizeDim& other) const noexcept = default;
};

[[nodiscard]] constexpr LIBV_FORCE_INLINE auto pixel(float value) noexcept {
	return SizeDim{value, 0.0f, 0.0f, false};
}
[[nodiscard]] constexpr LIBV_FORCE_INLINE auto percent(float value) noexcept {
	return SizeDim{0.0f, value, 0.0f, false};
}
[[nodiscard]] constexpr LIBV_FORCE_INLINE auto ratio(float value = 1.0f) noexcept {
	return SizeDim{0.0f, 0.0f, value, false};
}
[[nodiscard]] constexpr LIBV_FORCE_INLINE auto dynamic() noexcept {
	return SizeDim{0.0f, 0.0f, 0.0f, true};
}

// -------------------------------------------------------------------------------------------------

struct Size {
	libv::vec3_t<SizeDim> value;

	constexpr LIBV_FORCE_INLINE Size() noexcept = default;
	constexpr LIBV_FORCE_INLINE Size(SizeDim x, SizeDim y, SizeDim z = SizeDim{}) noexcept :
		value(std::move(x), std::move(y), std::move(z)) {}

	[[nodiscard]] constexpr LIBV_FORCE_INLINE SizeDim& operator[](std::size_t dim) noexcept {
		return value[dim];
	}
	[[nodiscard]] constexpr LIBV_FORCE_INLINE const SizeDim& operator[](std::size_t dim) const noexcept {
		return value[dim];
	}

	[[nodiscard]] constexpr LIBV_FORCE_INLINE bool has_dynamic() const noexcept {
		return value.x.dynamic || value.y.dynamic || value.z.dynamic;
	}

	[[nodiscard]] constexpr LIBV_FORCE_INLINE libv::vec2f pixel2() const noexcept {
		return {value.x.pixel, value.y.pixel};
	}
	[[nodiscard]] constexpr LIBV_FORCE_INLINE libv::vec2f percent2() const noexcept {
		return {value.x.percent, value.y.percent};
	}
	[[nodiscard]] constexpr LIBV_FORCE_INLINE libv::vec2f ratio2() const noexcept {
		return {value.x.ratio, value.y.ratio};
	}
	[[nodiscard]] constexpr LIBV_FORCE_INLINE libv::vec2f ratio_mask2() const noexcept {
		return {
			libv::float_equal(value.x.ratio, 0.f) ? 0.f : 1.f,
			libv::float_equal(value.y.ratio, 0.f) ? 0.f : 1.f
		};
	}
	[[nodiscard]] constexpr LIBV_FORCE_INLINE libv::vec2f dynamic_mask2() const noexcept {
		return {
			value.x.dynamic ? 1.f : 0.f,
			value.y.dynamic ? 1.f : 0.f
		};
	}

	template <typename OStream>
	friend LIBV_FORCE_INLINE OStream& operator<<(OStream& os, const Size& size) {
		os << size.value;
		return os;
	}

	[[nodiscard]] constexpr LIBV_FORCE_INLINE bool operator==(const Size& other) const noexcept = default;
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
