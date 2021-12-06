// Project: libv.ui, File: src/libv/ui/property/size.hpp

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
	bool dynamic = false; /// Yield all available space and shrink to the dynamic size

	inline SizeDim() = default;
	inline SizeDim(float pixel, float percent, float ratio, bool dynamic) :
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

	[[nodiscard]] inline bool operator==(const SizeDim& other) const noexcept = default;
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
inline auto dynamic() {
	return SizeDim{0.0f, 0.0f, 0.0f, true};
}

// -------------------------------------------------------------------------------------------------

struct Size {
	libv::vec3_t<SizeDim> value;

	constexpr inline Size() = default;
	constexpr inline Size(SizeDim x, SizeDim y, SizeDim z = SizeDim{}) :
		value{std::move(x), std::move(y), std::move(z)} {}

	[[nodiscard]] SizeDim& operator[](std::size_t dim) {
		return value[dim];
	}
	[[nodiscard]] const SizeDim& operator[](std::size_t dim) const {
		return value[dim];
	}

	[[nodiscard]] constexpr inline bool has_dynamic() const noexcept {
		return value.x.dynamic || value.y.dynamic || value.z.dynamic;
	}

	template <typename OStream>
	friend OStream& operator<<(OStream& os, const Size& size) {
		os << size.value;
		return os;
	}

	[[nodiscard]] inline bool operator==(const Size& other) const noexcept = default;
};

// -------------------------------------------------------------------------------------------------

//	struct SizeStat {
//		float fix = 0.f;
//		float percent = 0.f;
//
//		void add(const SizeStat& stat) {
//			fix += stat.fix;
//			percent += stat.percent;
//		}
//
//		void add(const Size& size, libv::vec3f dynamic_, uint32_t dim) {
//			fix += size[dim].pixel + (size[dim].dynamic ? dynamic_[dim] : 0.f);
//			percent += size[dim].percent;
//		}
//
//		void max(const Size& size, libv::vec3f dynamic_, uint32_t dim) {
//			fix = std::max(fix, size[dim].pixel + (size[dim].dynamic ? dynamic_[dim] : 0.f));
//			percent = std::max(percent, size[dim].percent);
//		}
//
//		float resolve(const CoreComponent& component) const {
//			if (fix < 0.01f) {
//				return fix;
//			} else if (percent > 99.99f) {
//				log_ui.warn("Invalid sum of size percent {} with fixed width of {} during layout of {}", percent, fix, component.path());
//				return fix * 2.f;
//			} else {
//				return fix / (1.f - percent * 0.01f);
//			}
//		}
//	};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
