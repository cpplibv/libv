// File: size.hpp Author: Vader Created on 2017. augusztus 17., 0:04

#pragma once

// libv
#include <libv/math/vec.hpp>
// std
#include <utility>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

struct SizeDim {
	float pixel = 0.0f;   /// Fixed size
	float percent = 0.0f; /// Parent size percentile
	float ratio = 1.0f;   /// Parent leftover space ratio
	bool content = false; /// Yield all available space and shrink to the content size

	inline SizeDim() = default;
	inline SizeDim(float pixel, float percent, float ratio, bool content) :
		pixel(pixel), percent(percent), ratio(ratio), content(content) { }

//	static inline SizeDim add(const SizeDim& a, const SizeDim& b) {
//		return SizeDim{
//			a.pixel + b.pixel,
//			a.percent + b.percent,
//			a.ratio + b.ratio,
//			a.content || b.content
//		};
//	}
//	static inline SizeDim max(const SizeDim& a, const SizeDim& b) {
//		return SizeDim{
//			std::max(a.pixel, b.pixel),
//			std::max(a.percent, b.percent),
//			std::max(a.ratio, b.ratio),
//			a.content || b.content
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
		if (size.content)
			os << "D";
		return os;
	}
};

inline auto pixel(float value) {
	return SizeDim{value, 0.0f, 0.0f, false};
}
inline auto percent(float value) {
	return SizeDim{0.0f, value, 0.0f, false};
}
inline auto ratio(float value = 1.0f) {
	return SizeDim{0.0f, 0.0f, value, false};
}
inline auto content() {
	return SizeDim{0.0f, 0.0f, 0.0f, true};
}

// -------------------------------------------------------------------------------------------------

struct Size {
	libv::vec3_t<SizeDim> value;

	constexpr inline Size() = default;
	constexpr inline explicit Size(SizeDim x, SizeDim y, SizeDim z = SizeDim{}) :
		value{std::move(x), std::move(y), std::move(z)} {}

	SizeDim& operator[](size_t dim) {
		return value[dim];
	}
	const SizeDim& operator[](size_t dim) const {
		return value[dim];
	}

	template <typename OStream>
	friend OStream& operator<<(OStream& os, const Size& size) {
		os << size.value;
		return os;
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
