// Project: libv.ui, File: src/libv/ui/component/layout/layout_slc.hpp

#pragma once

// libv
#include <libv/math/vec.hpp>
#include <libv/meta/force_inline.hpp>
#include <libv/utility/float_equal.hpp>
// pro
#include <libv/ui/component/component.hpp>
#include <libv/ui/context/context_layout.hpp>
#include <libv/ui/property/margin.hpp>
#include <libv/ui/property/orientation.hpp>
#include <libv/ui/property/padding.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

struct SLC_Node {
protected:
	libv::vec2f ratio_sum;
	libv::vec2f current_size;

public:
	[[nodiscard]] constexpr LIBV_FORCE_INLINE libv::vec2f ratio() const {
		return ratio_sum;
	}

	[[nodiscard]] constexpr LIBV_FORCE_INLINE libv::vec2f size() const {
		return current_size;
	}

public:
	/// Calculates and caches the ratio associated with the node into ratio_sum
	/// \return The ratio associated with the node
	[[nodiscard]] constexpr LIBV_FORCE_INLINE libv::vec2f calculate_ratio() {
		return {};
	}

	/// Assign the pixel part of the size to the node
	/// \param area_size The size of the parent
	/// \return the change in size compared to the previous assigment phase
	[[nodiscard]] constexpr LIBV_FORCE_INLINE libv::vec2f assign_pixel_size(libv::vec2f area_size) {
		(void) area_size;
		return {};
	}

	/// Assign the percent part of the size to the node
	/// \param area_size The size of the parent
	/// \return the change in size compared to the previous assigment phase
	[[nodiscard]] constexpr LIBV_FORCE_INLINE libv::vec2f assign_percent_size(libv::vec2f area_size) {
		(void) area_size;
		return {};
	}

	/// Assign the dynamic part of the size to the node
	/// \param area_size The maximum size used by the node (including the already allocated size)
	/// \return the change in size compared to the previous assigment phase
	[[nodiscard]] constexpr LIBV_FORCE_INLINE libv::vec2f assign_dynamic_size(libv::vec2f area_size) {
		(void) area_size;
		return {};
	}

	/// Assign the ratio part of the size to the node
	/// \param area_size The maximum size used by the node (including the already allocated size)
	/// 		based on its ratio values if it has any
	/// \return the change in size compared to the previous assigment phase
	[[nodiscard]] constexpr LIBV_FORCE_INLINE libv::vec2f assign_ratio_size(libv::vec2f area_size) {
		(void) area_size;
		return {};
	}

	/// Assign the final size and position of a node
	/// \param area_position The position of the area that was assigned to the node
	/// \param area_size The size of the area that was assigned to the node
	/// \param environment The layout context used for first class component recursion
	constexpr LIBV_FORCE_INLINE void assign_position(libv::vec2f area_position, libv::vec2f area_size, const ContextLayout2& environment) {
		(void) area_position;
		(void) area_size;
		(void) environment;
	}
};

// -------------------------------------------------------------------------------------------------

struct SLC_Component : SLC_Node {
	Component& component;

public:
	explicit constexpr LIBV_FORCE_INLINE SLC_Component(Component& component) :
		component(component) {}

public:
	[[nodiscard]] libv::vec2f calculate_ratio() {
		return ratio_sum = component.size().ratio2();
	}

	[[nodiscard]] libv::vec2f assign_pixel_size(libv::vec2f area_size) {
		(void) area_size;

		// const auto change = component.size().pixel2() + component.margin_extent().size();
		const auto change = component.size().pixel2();
		current_size += change;
		return change;
	}
	[[nodiscard]] libv::vec2f assign_percent_size(libv::vec2f area_size) {
		const auto change = component.size().percent2() / 100.f * area_size;
		current_size += change;
		return change;
	}
	[[nodiscard]] libv::vec2f assign_dynamic_size(libv::vec2f area_size) {
		if (!component.size().has_dynamic())
			return libv::vec2f{};

		const auto dynamic = xy(AccessLayout::layout1(ref_core(component), ContextLayout1{area_size}));
		const auto new_size = libv::max(current_size, dynamic * component.size().dynamic_mask2());
		const auto change = new_size - current_size;
		current_size = new_size;
		return change;
	}
	[[nodiscard]] libv::vec2f assign_ratio_size(libv::vec2f area_size) {
		const auto change = component.size().ratio_mask2() * (area_size - current_size);
		current_size += change;
		return change;
	}

	void assign_position(libv::vec2f area_position, libv::vec2f area_size, const ContextLayout2& environment) {
		// component.layout_position = position + component.margin.LB();
		// component.layout_position = area_position;
		// component.layout_size = current_size;

		// area_position += component.margin_extent().BL();
		// area_size += component.margin_extent().size();

		(void) area_size;

		const auto roundedPosition = libv::vec::round(area_position);
		const auto roundedSize = libv::vec::round(area_position + current_size) - roundedPosition;

		AccessLayout::layout2(ref_core(component), environment.enter(roundedPosition, roundedSize));
	}
};

// -------------------------------------------------------------------------------------------------

template <typename T>
struct SLC_Padding : SLC_Node {
	PaddingExtent padding;
	T child;

public:
	constexpr LIBV_FORCE_INLINE SLC_Padding(PaddingExtent padding, T child) :
		padding(padding),
		child(std::move(child)) {}

public:
	[[nodiscard]] constexpr libv::vec2f calculate_ratio() {
		return ratio_sum = child.calculate_ratio();
	}

	[[nodiscard]] constexpr libv::vec2f assign_pixel_size(libv::vec2f area_size) {
		const auto change =
				padding.size() +
				child.assign_pixel_size(area_size - padding.size());
		current_size += change;
		return change;
	}
	[[nodiscard]] constexpr libv::vec2f assign_percent_size(libv::vec2f area_size) {
		const auto change = child.assign_percent_size(area_size - padding.size());
		current_size += change;
		return change;
	}
	[[nodiscard]] constexpr libv::vec2f assign_dynamic_size(libv::vec2f area_size) {
		const auto change = child.assign_dynamic_size(area_size - padding.size());
		current_size += change;
		return change;
	}
	[[nodiscard]] constexpr libv::vec2f assign_ratio_size(libv::vec2f area_size) {
		const auto change = child.assign_ratio_size(area_size - padding.size());
		current_size += change;
		return change;
	}

	constexpr void assign_position(libv::vec2f area_position, libv::vec2f area_size, const ContextLayout2& environment) {
		child.assign_position(area_position + padding.LB(), area_size - padding.size(), environment);
	}
};

namespace detail { // ------------------------------------------------------------------------------

struct OrientationDataSCLLine {
	uint32_t primary;
	uint32_t secondary;
	libv::vec2f penStart;
	libv::vec2f penCorner;
	libv::vec2f direction;
};

static_assert(libv::to_underlying(Orientation::up) == 0u);
static_assert(libv::to_underlying(Orientation::right) == 1u);
static_assert(libv::to_underlying(Orientation::left) == 2u);
static_assert(libv::to_underlying(Orientation::down) == 3u);
static constexpr OrientationDataSCLLine OrientationTableSCLLine[] = {
//       Pri Sec  PenStart    PenCorner     Direction
		{1u, 0u, {0.f, 0.f}, {+0.f, +0.f}, {+1.f, +1.f}}, // Orientation::up;
		{0u, 1u, {0.f, 0.f}, {+0.f, +0.f}, {+1.f, +1.f}}, // Orientation::right;
		{0u, 1u, {1.f, 0.f}, {-1.f, +0.f}, {-1.f, +1.f}}, // Orientation::left;
		{1u, 0u, {0.f, 1.f}, {+0.f, -1.f}, {+1.f, -1.f}}, // Orientation::down;
};

} // namespace detail ------------------------------------------------------------------------------

///
/// Expected CRTP methods:
/// 	std::size_t count_children() const noexcept;
/// 	template <typename F> void foreach_children(F&& func) noexcept;
///
template <typename CRTP>
struct SLC_Line : SLC_Node {
private:
	const uint32_t primary;
	const uint32_t secondary;
	const float spacing;
	const Orientation orientation;

public:
	constexpr LIBV_FORCE_INLINE SLC_Line(Orientation orientation, float spacing) :
		primary(detail::OrientationTableSCLLine[+orientation].primary),
		secondary(detail::OrientationTableSCLLine[+orientation].secondary),
		spacing(spacing),
		orientation(orientation) {
	}

private:
	[[nodiscard]] constexpr LIBV_FORCE_INLINE CRTP& crtp() noexcept {
		return static_cast<CRTP&>(*this);
	}
	[[nodiscard]] constexpr LIBV_FORCE_INLINE const CRTP& crtp() const noexcept {
		return static_cast<const CRTP&>(*this);
	}

public:
	template <typename T>
	[[nodiscard]] constexpr LIBV_FORCE_INLINE libv::vec2_t<T> add_primary_max_secondary(libv::vec2_t<T> a, libv::vec2_t<T> b) const noexcept {
		return swizzle_to_vec2(
				primary, a[primary] + b[primary],
				secondary, std::max(a[secondary], b[secondary]));
	}

	[[nodiscard]] constexpr libv::vec2f calculate_ratio() {
		crtp().foreach_children([&](auto& child) {
			ratio_sum = add_primary_max_secondary(ratio_sum, child.calculate_ratio());
		});
		return ratio_sum;
	}

	[[nodiscard]] constexpr libv::vec2f assign_pixel_size(libv::vec2f area_size) {
		libv::vec2f change;
		change[primary] += spacing * static_cast<float>(std::max(crtp().count_children(), 1uz) - 1uz);
		crtp().foreach_children([&](auto& child) {
			 change = add_primary_max_secondary(change, child.assign_pixel_size(area_size));
		});
		current_size += change;
		return change;
	}

	[[nodiscard]] constexpr libv::vec2f assign_percent_size(libv::vec2f area_size) {
		libv::vec2f change;
		crtp().foreach_children([&](auto& child) {
			 change = add_primary_max_secondary(change, child.assign_percent_size(area_size));
		});
		current_size += change;
		return change;
	}

	[[nodiscard]] constexpr libv::vec2f assign_dynamic_size(libv::vec2f area_size) {
		auto unused = area_size[primary] - current_size[primary];

		libv::vec2f change;
		crtp().foreach_children([&](auto& child) {
			const auto child_area = swizzle_to_vec2(
					primary, child.size()[primary] + unused,
					secondary, area_size[secondary]);
			const auto child_change = child.assign_dynamic_size(child_area);
			unused -= child_change[primary];
			change = add_primary_max_secondary(change, child_change);
		});
		current_size += change;
		return change;
	}

	[[nodiscard]] constexpr libv::vec2f assign_ratio_size(libv::vec2f area_size) {
		libv::vec2f change;

		const auto ratio_scale = libv::float_equal(ratio_sum[primary], 0.f) ?
				0.f :
				(area_size[primary] - current_size[primary]) / ratio_sum[primary];
		crtp().foreach_children([&](auto& child) {
			const auto child_area = swizzle_to_vec2(
					primary, child.size()[primary] + ratio_scale * child.ratio()[primary],
					secondary, area_size[secondary]);
			const auto child_change = child.assign_ratio_size(child_area);
			change = add_primary_max_secondary(change, child_change);
		});
		current_size += change;
		return change;
	}

	constexpr void assign_position(libv::vec2f area_position, libv::vec2f area_size, const ContextLayout2& environment) {
		const auto& orient = detail::OrientationTableSCLLine[+orientation];

		// libv::vec2f pen = area_position + orient.start * area_size;
		// TODO P4: Alignment / Anchor, Currently I am aligning content to bottom left with 'content size'
		libv::vec2f pen = area_position + orient.penStart * current_size;

		crtp().foreach_children([&](auto& child) {
			const auto size = child.size();
			child.assign_position(
					pen + orient.penCorner * size,
					swizzle_to_vec2(
						primary, size[primary],
						secondary, area_size[secondary]),
					environment);

			pen[primary] += orient.direction[primary] * (size[primary] + spacing);
		});
	}
};

template <typename... Children>
struct SLC_LineStatic : SLC_Line<SLC_LineStatic<Children...>> {
	std::tuple<Children...> children;

public:
	constexpr LIBV_FORCE_INLINE SLC_LineStatic(float spacing, Orientation orientation, Children... children) :
		SLC_Line<SLC_LineStatic<Children...>>(orientation, spacing),
		children(std::move(children)...) {}

public:
	[[nodiscard]] constexpr LIBV_FORCE_INLINE std::size_t count_children() const noexcept {
		return sizeof...(Children);
	}

	template <typename F>
	constexpr LIBV_FORCE_INLINE void foreach_children(F&& func) noexcept {
		std::apply([&](auto&... child) {
			(func(child), ...);
		}, children);
	}
};

// -------------------------------------------------------------------------------------------------

template <typename T>
[[nodiscard]] constexpr libv::vec2f layoutSLCPass1(libv::vec2f size, T&& plan) {
	libv::vec2f result;
	result += plan.assign_pixel_size(size);
	result += plan.assign_dynamic_size(size);
	return result;
}

template <typename T>
[[nodiscard]] constexpr libv::vec2f layoutSLCPass1(libv::vec3f size, T&& plan) {
	return layoutSLCPass1(xy(size), std::forward<T>(plan));
}

template <typename T>
constexpr void layoutSLCPass2(libv::vec2f size, const ContextLayout2& environment, T&& plan) {
	(void) plan.calculate_ratio();

	(void) plan.assign_pixel_size(size);
	(void) plan.assign_percent_size(size);
	(void) plan.assign_dynamic_size(size);
	(void) plan.assign_ratio_size(size);

	plan.assign_position({0, 0}, size, environment);
}

template <typename T>
constexpr void layoutSLCPass2(libv::vec3f size, const ContextLayout2& environment, T&& plan) {
	layoutSLCPass2(xy(size), environment, std::forward<T>(plan));
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
