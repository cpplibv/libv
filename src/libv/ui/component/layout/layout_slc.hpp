// Project: libv.ui, File: src/libv/ui/component/layout/layout_slc.hpp

#pragma once

// libv
#include <libv/math/vec.hpp>
#include <libv/meta/force_inline.hpp>
#include <libv/utility/float_equal.hpp>
// pro
#include <libv/ui/component/component.hpp>
#include <libv/ui/component/layout/layout_utility.hpp>
#include <libv/ui/context/context_layout.hpp>
#include <libv/ui/property/align_horizontal.hpp>
#include <libv/ui/property/align_vertical.hpp>
#include <libv/ui/property/margin.hpp>
#include <libv/ui/property/orientation.hpp>
#include <libv/ui/property/padding.hpp>
// std
#include <span>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class SLCSizeConstraints {
private:
	/// Indicator if the given direction is unlimited or not.
	/// Unbounded dimensions can ignore limit and occupy as much space as they want.
	libv::vec2b unlimited_;
	/// Max size that a node can acquire (negative values represents oversized children).
	/// Includes the space that was already assigned to the component.
	libv::vec2f limit_;
	/// Parent size (never infinite or negative). Falls back to parent size if parent is not constrained.
	libv::vec2f parent_;

public:
	SLCSizeConstraints(libv::vec2b unlimited, libv::vec2f limit, libv::vec2f parent) :
		unlimited_(unlimited),
		limit_(limit),
		parent_(parent) {
	}

	[[nodiscard]] constexpr libv::vec2b unlimited() const {
		return unlimited_;
	}
	[[nodiscard]] constexpr libv::vec2f limit() const {
		return limit_;
	}
	[[nodiscard]] constexpr libv::vec2f parent() const {
		return parent_;
	}

public:
	[[nodiscard]] constexpr SLCSizeConstraints reduce(const libv::vec2f pixels) const {
		return {unlimited_, limit_ - pixels, parent_};
	}
	constexpr void exclude(const libv::vec2f pixels) {
		parent_ -= pixels;
	}
};

// -------------------------------------------------------------------------------------------------

struct SLC_Node {
protected:
	libv::vec2f ratioSum;
	libv::vec2f currentSize;

public:
	[[nodiscard]] constexpr LIBV_FORCE_INLINE libv::vec2f ratio() const {
		return ratioSum;
	}

	[[nodiscard]] constexpr LIBV_FORCE_INLINE libv::vec2f size() const {
		return currentSize;
	}

public:
	// /// Calculates and caches the ratio associated with the node into ratioSum
	// /// \return The ratio associated with the node
	// [[nodiscard]] constexpr LIBV_FORCE_INLINE libv::vec2f calculate_ratio();
	//
	// /// Assign the pixel part of the size to the node
	// /// \param area_size The size of the parent
	// /// \return the change in size compared to the previous assigment phase
	// constexpr LIBV_FORCE_INLINE void assign_pixel_size(const SLCSizeConstraints& env);
	//
	// /// Assign the percent part of the size to the node
	// /// \param area_size The size of the parent
	// /// \return the change in size compared to the previous assigment phase
	// constexpr LIBV_FORCE_INLINE void assign_percent_size(libv::vec2f area_size);
	//
	// /// Assign the dynamic part of the size to the node
	// /// \param area_size The maximum size used by the node (including the already allocated size)
	// /// \return the change in size compared to the previous assigment phase
	// constexpr LIBV_FORCE_INLINE void assign_dynamic_size(libv::vec2f area_size);
	//
	// /// Assign the ratio part of the size to the node
	// /// \param area_size The maximum size used by the node (including the already allocated size)
	// /// 		based on its ratio values if it has any
	// /// \return the change in size compared to the previous assigment phase
	// constexpr LIBV_FORCE_INLINE void assign_ratio_size(libv::vec2f area_size);
	//
	// /// Assign the final size and position of a node
	// /// \param area_position The position of the area that was assigned to the node
	// /// \param area_size The size of the area that was assigned to the node
	// /// \param environment The layout context used for first class component recursion
	// constexpr LIBV_FORCE_INLINE void assign_position(libv::vec2f area_position, libv::vec2f area_size, const ContextLayout2& environment);
};

// -------------------------------------------------------------------------------------------------

struct SLC_Component : SLC_Node {
	Component& component;

public:
	explicit constexpr LIBV_FORCE_INLINE SLC_Component(Component& component) :
		component(component) {}

public:
	[[nodiscard]] constexpr libv::vec2f calculate_ratio() {
		return ratioSum = component.size().ratio2();
	}
	[[nodiscard]] constexpr libv::vec2f calculate_exclude() {
		return currentSize = {};
	}

	constexpr void assign_primary_size(const SLCSizeConstraints& env) {
		currentSize = component.size().pixel2();

		const auto eval = [](float& current, const SizeDim& size, float value) {
			if (libv::float_equal(size.percent, 0.f))
				return;

			switch (size.mode) {
			case SizeDim::Mode::max:
				current = std::max(current, value);
				break;
			case SizeDim::Mode::min:
				current = std::min(current, value);
				break;
			}
		};

		const auto percent = component.size().percent2() * 0.01f * env.parent();
		eval(currentSize.x, component.size().value.x, percent.x);
		eval(currentSize.y, component.size().value.y, percent.y);
	}

	constexpr void assign_dynamic_size(const SLCSizeConstraints& env) {
		if (!component.size().has_dynamic())
			return;

		const auto eval = [](float& current, const SizeDim& size, float value) {
			if (!size.dynamic)
				return;

			switch (size.mode) {
			case SizeDim::Mode::max:
				current = std::max(current, value);
				break;
			case SizeDim::Mode::min:
				current = std::min(current, value);
				break;
			}
		};

		const auto limit = libv::vec2f{
			component.size().value.x.has_ratio() || component.size().value.x.dynamic ? env.limit().x : currentSize.x,
			component.size().value.y.has_ratio() || component.size().value.y.dynamic ? env.limit().y : currentSize.y
		};
		const auto unlimited = component.size().dynamic2() && env.unlimited();
		const auto dynamic = AccessLayout::layout1(ref_core(component), ContextLayout1{unlimited, limit, env.parent()});
		eval(currentSize.x, component.size().value.x, dynamic.x);
		eval(currentSize.y, component.size().value.y, dynamic.y);
	}

	constexpr void assign_ratio_size(const SLCSizeConstraints& env) {
		currentSize = libv::max(currentSize, env.limit() * component.size().ratio_mask2());
	}

	void assign_position(libv::vec2f area_position, libv::vec2f area_size, const ContextLayout2& environment) {
		(void) area_size;

		const auto roundedPosition = libv::vec::round(area_position);
		const auto roundedSize = libv::vec::round(area_position + currentSize) - roundedPosition;

		AccessLayout::layout2(ref_core(component), environment.enter(roundedPosition, roundedSize));
	}
};

// -------------------------------------------------------------------------------------------------

template <typename T>
struct SLC_Margin : SLC_Node {
	libv::vec2f marginSize;
	libv::vec2f marginLB;
	T child;

public:
	constexpr LIBV_FORCE_INLINE SLC_Margin(MarginExtent margin, T child) :
		marginSize(margin.size()),
		marginLB(margin.LB()),
		child(std::move(child)) {}

public:
	[[nodiscard]] constexpr libv::vec2f calculate_ratio() {
		return ratioSum = child.calculate_ratio();
	}

	[[nodiscard]] constexpr libv::vec2f calculate_exclude() {
		return currentSize = child.calculate_exclude() + marginSize;
	}
	constexpr void assign_primary_size(const SLCSizeConstraints& env) {
		child.assign_primary_size(env.reduce(marginSize));
		currentSize = child.size() + marginSize;
	}
	constexpr void assign_dynamic_size(const SLCSizeConstraints& env) {
		child.assign_dynamic_size(env.reduce(marginSize));
		currentSize = child.size() + marginSize;
	}
	constexpr void assign_ratio_size(const SLCSizeConstraints& env) {
		child.assign_ratio_size(env.reduce(marginSize));
		currentSize = child.size() + marginSize;
	}

	constexpr void assign_position(libv::vec2f area_position, libv::vec2f area_size, const ContextLayout2& environment) {
		child.assign_position(area_position + marginLB, area_size - marginSize, environment);
	}
};

// -------------------------------------------------------------------------------------------------

template <typename T>
struct SLC_Padding : SLC_Node {
	libv::vec2f paddingSize;
	libv::vec2f paddingLB;
	T child;

public:
	constexpr LIBV_FORCE_INLINE SLC_Padding(PaddingExtent padding, T child) :
		paddingSize(padding.size()),
		paddingLB(padding.LB()),
		child(std::move(child)) {}

public:
	[[nodiscard]] constexpr libv::vec2f calculate_ratio() {
		return ratioSum = child.calculate_ratio();
	}
	[[nodiscard]] constexpr libv::vec2f calculate_exclude() {
		return currentSize = child.calculate_exclude() + paddingSize;
	}
	constexpr void assign_primary_size(const SLCSizeConstraints& env) {
		child.assign_primary_size(env.reduce(paddingSize));
		currentSize = child.size() + paddingSize;
	}
	constexpr void assign_dynamic_size(const SLCSizeConstraints& env) {
		child.assign_dynamic_size(env.reduce(paddingSize));
		currentSize = child.size() + paddingSize;
	}
	constexpr void assign_ratio_size(const SLCSizeConstraints& env) {
		child.assign_ratio_size(env.reduce(paddingSize));
		currentSize = child.size() + paddingSize;
	}

	constexpr void assign_position(libv::vec2f area_position, libv::vec2f area_size, const ContextLayout2& environment) {
		child.assign_position(area_position + paddingLB, area_size - paddingSize, environment);
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

static_assert(+Orientation::up == 0u);
static_assert(+Orientation::right == 1u);
static_assert(+Orientation::left == 2u);
static_assert(+Orientation::down == 3u);
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
	const AlignHorizontal align_horizontal;
	const AlignVertical align_vertical;

	libv::vec2f spacingSum;

public:
	constexpr LIBV_FORCE_INLINE SLC_Line(Orientation orientation, float spacing, AlignHorizontal align_horizontal,
			AlignVertical align_vertical) :
		primary(detail::OrientationTableSCLLine[+orientation].primary),
		secondary(detail::OrientationTableSCLLine[+orientation].secondary),
		spacing(spacing),
		orientation(orientation),
		align_horizontal(align_horizontal),
		align_vertical(align_vertical) {
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
	constexpr LIBV_FORCE_INLINE void primary_add_secondary_max(libv::vec2_t<T>& a, libv::vec2_t<T> b) const noexcept {
		a[primary] += b[primary];
		a[secondary] = std::max(a[secondary], b[secondary]);
	}

	[[nodiscard]] constexpr libv::vec2f calculate_ratio() {
		crtp().foreach_children([&](auto& child) {
			primary_add_secondary_max(ratioSum, child.calculate_ratio());
		});
		return ratioSum;
	}

	[[nodiscard]] constexpr libv::vec2f calculate_exclude() {
		const auto numSpaces = static_cast<float>(std::max(crtp().count_children(), 1uz) - 1uz);
		spacingSum = swizzle_to_vec2(primary, spacing * numSpaces, secondary, 0.f);
		currentSize = spacingSum;

		crtp().foreach_children([&](auto& child) {
			primary_add_secondary_max(currentSize, child.calculate_exclude());
		});

		return currentSize;
	}
	constexpr void assign_primary_size(const SLCSizeConstraints& env) {
		auto remainingSpacePrimary = env.limit()[primary] - currentSize[primary];

		currentSize = spacingSum;
		crtp().foreach_children([&](auto& child) {
			remainingSpacePrimary += child.size()[primary];
			auto childEnv = SLCSizeConstraints{
					env.unlimited(),
					swizzle_to_vec2(primary, remainingSpacePrimary, secondary, env.limit()[secondary]),
					env.parent()};
			child.assign_primary_size(childEnv);
			primary_add_secondary_max(currentSize, child.size());
			remainingSpacePrimary -= child.size()[primary];
		});
	}
	constexpr void assign_dynamic_size(const SLCSizeConstraints& env) {
		auto remainingSpacePrimary = env.limit()[primary] - currentSize[primary];
		const auto hasRatioLinePrimary = libv::float_not_equal(ratioSum[primary], 0.f);

		currentSize = spacingSum;
		crtp().foreach_children([&](auto& child) {
			remainingSpacePrimary += child.size()[primary];
			const auto hasRatioChildPrimary = libv::float_not_equal(child.ratio()[primary], 0.f);
			const auto limit = swizzle_to_vec2(
					primary, hasRatioLinePrimary && hasRatioChildPrimary ? remainingSpacePrimary * child.ratio()[primary] / ratioSum[primary] : remainingSpacePrimary,
					secondary, env.limit()[secondary]);
			// TODO P5: Limit is incorrect, it does not account for children with: 1r and 1rD sizes in primary
			//			and does not rewards the full limit the children with dyanmic in primary
			//			We could solve this by doing 2 iteration within this assign_dynamic_size on children
			//			iteration 1: children with dyanmic in primary, iteration 2: everyone else
			auto childEnv = SLCSizeConstraints{env.unlimited(), limit, env.parent()};
			child.assign_dynamic_size(childEnv);
			primary_add_secondary_max(currentSize, child.size());
			remainingSpacePrimary -= child.size()[primary];
		});
	}
	constexpr void assign_ratio_size(const SLCSizeConstraints& env) {
		const auto remainingSpacePrimary = env.limit()[primary] - currentSize[primary];
		const auto ratioScalePrimary = libv::float_equal(ratioSum[primary], 0.f) ? 0.f :
				std::max(remainingSpacePrimary / ratioSum[primary], 0.f);

		currentSize = spacingSum;
		crtp().foreach_children([&](auto& child) {
			const auto limit = swizzle_to_vec2(
					primary, child.size()[primary] + ratioScalePrimary * child.ratio()[primary],
					secondary, env.limit()[secondary]);
			auto childEnv = SLCSizeConstraints{env.unlimited(), limit, env.parent()};
			child.assign_ratio_size(childEnv);
			primary_add_secondary_max(currentSize, child.size());
		});
	}

	constexpr void assign_position(libv::vec2f area_position, libv::vec2f area_size, const ContextLayout2& environment) {
		const auto& orient = detail::OrientationTableSCLLine[+orientation];

		// Content / Unused
		auto contentSize = currentSize;
		auto unusedSpace = area_size - contentSize;

		// Justify
		const auto isJustified = crtp().count_children() >= 2 && (primary == 0 ? info(align_horizontal).justified() : info(align_vertical).justified());
		const auto spaceDistributedForJustifyPrimary = unusedSpace[primary];
		const auto justifyGapPrimary = isJustified ? spaceDistributedForJustifyPrimary / static_cast<float>(crtp().count_children() - 1) : 0.f;
		if (isJustified) {
			unusedSpace[primary] = 0.f;
			contentSize[primary] = area_size[primary];
		}

		// Alignment
		const auto alignmentScale = libv::vec2f(info(align_horizontal).rate(), info(align_vertical).rate());

		// Assign
		const auto originToContent = unusedSpace * alignmentScale;
		const auto contentToStart = contentSize * orient.penStart;

		auto startToPen = vec2f{};

		crtp().foreach_children([&](auto& child) {
			const auto penToBase = child.size() * orient.penCorner;

			const auto unusedSpaceAboveChildSecondary = contentSize[secondary] - child.size()[secondary];
			auto baseToPosition = vec2f{};
			baseToPosition[secondary] = unusedSpaceAboveChildSecondary * alignmentScale[secondary];

			const auto position =
					area_position
					+ originToContent
					+ contentToStart
					+ startToPen
					+ penToBase
					+ baseToPosition;

			child.assign_position(position, child.size(), environment);

			startToPen[primary] += (child.size()[primary] + spacing + justifyGapPrimary) * orient.direction[primary];
		});
	}
};

template <typename... Children>
struct SLC_LineStatic : SLC_Line<SLC_LineStatic<Children...>> {
	std::tuple<Children...> children;

public:
	constexpr LIBV_FORCE_INLINE SLC_LineStatic(float spacing, Orientation orientation, AlignHorizontal align_horizontal,
			AlignVertical align_vertical, Children... children) :
		SLC_Line<SLC_LineStatic<Children...>>(orientation, spacing, align_horizontal, align_vertical),
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

template <typename ChildType>
struct SLC_LineDynamic : SLC_Line<SLC_LineDynamic<ChildType>> {
	std::span<ChildType> children;

public:
	constexpr LIBV_FORCE_INLINE SLC_LineDynamic(float spacing, Orientation orientation, AlignHorizontal align_horizontal,
			AlignVertical align_vertical, std::span<ChildType> children) :
		SLC_Line<SLC_LineDynamic<ChildType>>(orientation, spacing, align_horizontal, align_vertical),
		children(children) {}

public:
	[[nodiscard]] LIBV_FORCE_INLINE std::size_t count_children() const noexcept {
		return children.size();
	}

	template <typename F>
	LIBV_FORCE_INLINE void foreach_children(F&& func) noexcept {
		for (auto& child : children)
			func(child);
	}
};

template <typename T>
struct SLC_ComponentAnchor : SLC_Node {

	Anchor source_anchor;
	Anchor target_anchor;

	Component& target;
	T child;

public:
	constexpr LIBV_FORCE_INLINE SLC_ComponentAnchor(
			Anchor source_anchor,
			Anchor target_anchor,
			Component& target,
			T child) :
		source_anchor(source_anchor),
		target_anchor(target_anchor),
		target(target),
		child(std::move(child)) {}

public:
	[[nodiscard]] constexpr libv::vec2f calculate_ratio() {
		return ratioSum = child.calculate_ratio();
	}

	[[nodiscard]] constexpr libv::vec2f calculate_exclude() {
		return currentSize = child.calculate_exclude();
	}
	constexpr void assign_primary_size(const SLCSizeConstraints& env) {
		child.assign_primary_size(env);
		currentSize = child.size();
	}
	constexpr void assign_dynamic_size(const SLCSizeConstraints& env) {
		child.assign_dynamic_size(env);
		currentSize = child.size();
	}
	constexpr void assign_ratio_size(const SLCSizeConstraints& env) {
		child.assign_ratio_size(env);
		currentSize = child.size();
	}

	void assign_position(libv::vec2f area_position, libv::vec2f area_size, const ContextLayout2& environment) {
		(void) area_position;
		(void) area_size;

		// Tooltip TODO P2: fetch absolute position, layout invalidation and movement tracking

		const auto target_pos = target.layout_position_absolute();
		const auto target_size = target.layout_size();
		const auto position = target_pos + target_size * info(target_anchor).rate() - currentSize * info(source_anchor).rate();

		child.assign_position(position, currentSize, environment);
	}
};

// -------------------------------------------------------------------------------------------------

template <typename Core>
[[nodiscard]] constexpr libv::vec2f layoutSLCCorePass1(Core& core, const ContextLayout1& ctx) {
	auto env = SLCSizeConstraints{ctx.unlimited, ctx.limit, ctx.parent};

	libv::vec2f result;
	core.access_layout([&](auto&& plan) {
		// No calculate_ratio in pass 1 mode
		env.exclude(plan.calculate_exclude()); // Margin, Padding, Spacing
		plan.assign_primary_size(env); // Pixel, Percent
		plan.assign_dynamic_size(env); // Dynamic
		// No assign_ratio_size in pass 1 mode

		// No assign_position in pass 1 mode
		result = plan.size();
	});
	return result;
}

template <typename Core>
constexpr void layoutSLCCorePass2(Core& core, const ContextLayout2& ctx) {
	const auto unlimited = libv::vec2b(false, false);
	const auto limit = ctx.size;
	const auto parent = ctx.size;
	auto env = SLCSizeConstraints{unlimited, limit, parent};

	core.access_layout([&](auto&& plan) {
		(void) plan.calculate_ratio();
		env.exclude(plan.calculate_exclude()); // Margin, Padding, Spacing
		plan.assign_primary_size(env); // Pixel, Percent
		plan.assign_dynamic_size(env); // Dynamic
		plan.assign_ratio_size(env); // Ratio

		plan.assign_position({0, 0}, ctx.size, ctx);
	});
}

// -------------------------------------------------------------------------------------------------

// TODO P5: Document SLC better, sizeing rules, max selection
// SLC - Static Layout Composition
//
// (false ATM) SLCSizeConstraints parent can never has unlimited or negative
// SLCSizeConstraints in layout pass 2 can never has unlimited limits (might be false in future)
//
// container spacing, container padding and child margin are not part of the size
//
// min/max/add modes for Pixel/Percent/Ratio
//
// 	// Pixel   -
// 	// Percent -
// 	// Dynamic -
// 	// Ratio   - Leftover unused space's ratio
//
// Line
//                       +--------------------------+     ^   ^
//                       | Padding Front            |     |   | Child layout position
//           ^ ^         |    Child Margin Front    |     |   | (The final value passed to the child layout2)
//           | |         |  +---------------------+ |     | ^ v
//           | |         |  | Child Padding Front | |     | |
//           | |         |  | Child Content       | |     | | Child layout Size
//           | |         |  | Child Padding Back  | |     | | (The final value passed to the child layout2)
//           | |         |  +---------------------+ |     | v
//           | v         |    Child Margin Back     |     |
//           |           | Spacing                  |     |
//           | ^         |    Child Margin Front    |     |
//           | |         |  +---------------------+ |     | ^
//           | |         |  | Child Padding Front | |     | |
//           | |         |  | Child Content       | |     | |
//           | |         |  | Child Padding Back  | |     | | Child layout Size
//           | |         |  +---------------------+ |     | v
//           v |         |    Child Margin Back     |     |
//             |         |                          |     |
//             |         | Unused space             |     |
//             v         |                          |     |
//            \ \        | Padding Back             |     | Layout Size
//             \ \       +--------------------------+     v
//              \ \
//               \  Size that is distributed by 'percent' type of children's size (Layout Size minus spacings and parent padding)
//                \
//                  Content area size (Layout Size minus parent padding and unused space)

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
