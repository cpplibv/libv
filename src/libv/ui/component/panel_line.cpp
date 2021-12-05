// Project: libv.ui, File: src/libv/ui/component/panel_line.cpp

// hpp
#include <libv/ui/component/panel_line.hpp>
#include <libv/ui/component/panel_line_core.hpp>
// ext
#include <boost/container/small_vector.hpp>
// libv
#include <libv/utility/to_underlying.hpp>
// std
#include <algorithm>
// pro
#include <libv/ui/component/base_panel_core.hpp>
#include <libv/ui/component/detail/core_component.hpp>
#include <libv/ui/component/layout/layout_utility.hxx>
#include <libv/ui/component/layout/view_layouted.hxx>
#include <libv/ui/context/context_layout.hpp>
#include <libv/ui/context/context_style.hpp>
#include <libv/ui/log.hpp>
#include <libv/ui/property.hpp>
#include <libv/ui/property_access_context.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

namespace {

struct OrientationData {
	uint32_t _X_;
	uint32_t _Y_;
	uint32_t _Z_;
	uint32_t inset_frontX;
	uint32_t inset_backX;
	uint32_t inset_topY;
	uint32_t inset_bottomY;
	libv::vec3f start;
	libv::vec3f direction;
	libv::vec3f penCorner;
};

static_assert(libv::to_underlying(Orientation::up) == 0u);
static_assert(libv::to_underlying(Orientation::right) == 1u);
static_assert(libv::to_underlying(Orientation::left) == 2u);
static_assert(libv::to_underlying(Orientation::down) == 3u);
// Inset: 0 = left, 1 = bottom, 2 = right, 3 = top
static constexpr OrientationData OrientationTable[] = {
//       X   Y   Z   Fro Bac Top Bot  Start            Direction           PenCorner
		{1u, 0u, 2u, 1u, 3u, 2u, 0u, {0.f, 0.f, 0.f}, {+1.f, +1.f, +1.f}, {+0.f, +0.f, +0.f}}, // Orientation::up;
		{0u, 1u, 2u, 0u, 2u, 3u, 1u, {0.f, 0.f, 0.f}, {+1.f, +1.f, +1.f}, {+0.f, +0.f, +0.f}}, // Orientation::right;
		{0u, 1u, 2u, 2u, 0u, 3u, 1u, {1.f, 0.f, 0.f}, {-1.f, +1.f, +1.f}, {-1.f, +0.f, +0.f}}, // Orientation::left;
		{1u, 0u, 2u, 3u, 1u, 2u, 0u, {0.f, 1.f, 0.f}, {+1.f, -1.f, +1.f}, {+0.f, -1.f, +0.f}}, // Orientation::down;
};

struct AlignmentData {
	float scale;
	bool justified;
};

} // namespace

// -------------------------------------------------------------------------------------------------

void CorePanelLine::doStyle(ContextStyle& ctx) {
	PropertyAccessContext<Properties> setter{property, ctx.component, ctx.style, ctx.component.context()};
	access_properties(setter);
	CoreBasePanel::doStyle(ctx);
}

void CorePanelLine::doStyle(ContextStyle& ctx, ChildID childID) {
	(void) ctx;
	(void) childID;
}

// -------------------------------------------------------------------------------------------------

libv::vec3f CorePanelLine::doLayout1(const ContextLayout1& layout_env) {
	const auto& orientData = OrientationTable[underlying(property.orientation())];
	const auto _X_ = orientData._X_;
	const auto _Y_ = orientData._Y_;

	// --- Sum: MarginX / SpacingX / Padding ---

	auto num_layouted_child = int32_t{0};
	auto margin_sumX = 0.f;

	for (auto& child : children | view_layouted()) {
		margin_sumX += child.margin_size3()[_X_];
		num_layouted_child++;
	}

	const auto spacing_sumX = property.spacing() * static_cast<float>(std::max(0, num_layouted_child - 1));

	const auto sum_marginX_spacingX_padding =
			build_vec3f(_X_, margin_sumX) +
			build_vec3f(_X_, spacing_sumX) +
			padding_size3();

	// --- Size pass 1 ---

	const auto base_content_area_size = layout_env.size - sum_marginX_spacingX_padding;

	auto pixelX = 0.f;
	auto percentX = 0.f;
	auto contentX = 0.f;
	auto resultX = 0.f;
	auto resultY = 0.f;

	for (auto& child : children | view_layouted()) {
		const auto child_max_size = base_content_area_size - child.margin_size3()[_Y_];
		const auto child_dynamic = calculate_layout1_contrib(child.core(), child_max_size);

		pixelX += child.size()[_X_].pixel;
		percentX += child.size()[_X_].percent;
		contentX += child_dynamic[_X_];

		resultY = std::max(resultY,
				resolvePercent(
					child.size()[_Y_].pixel + child_dynamic[_Y_],
					child.size()[_Y_].percent, child)
				+ child.margin_size()[_Y_]);
	}

	resultX = resolvePercent(pixelX + contentX, percentX, *this);

	// --- Sum ---

	const auto result = build_vec3f(_X_, resultX, _Y_, resultY, 2, 0.f);

	return result + sum_marginX_spacingX_padding;
}

void CorePanelLine::doLayout2(const ContextLayout2& layout_env) {
	const auto& alignHData = AlignmentData(info(property.align_horizontal()).rate(), info(property.align_horizontal()).justified());
	const auto& alignVData = AlignmentData(info(property.align_vertical()).rate(), info(property.align_vertical()).justified());
	const auto& orientData = OrientationTable[underlying(property.orientation())];

	//
	//                       +-------------------------+     ^   ^
	//                       | Padding Front           |     |   | Child layout position
	//           ^ ^         |    Child Margin Front   |     |   | (The final value passed to the child layout2)
	//           | |         |  +---------------------+|     | ^ v
	//           | |         |  | Child Padding Front ||     | |
	//           | |         |  | Child Content       ||     | | Child layout Size
	//           | |         |  | Child Padding Back  ||     | | (The final value passed to the child layout2)
	//           | |         |  +---------------------+|     | v
	//           | v         |    Child Margin Back    |     |
	//           |           | Spacing                 |     |
	//           | ^         |    Child Margin Front   |     |
	//           | |         |  +---------------------+|     | ^
	//           | |         |  | Child Padding Front ||     | |
	//           | |         |  | Child Content       ||     | |
	//           | |         |  | Child Padding Back  ||     | | Child layout Size
	//           | |         |  +---------------------+|     | v
	//           v |         |    Child Margin Back    |     |
	//             |         |                         |     |
	//             |         | Unused space            |     |
	//             v         |                         |     |
	//            \ \        | Padding Back            |     | Layout Size
	//             \ \       +-------------------------+     v
	//              \ \
	//               \  Size that is distributed by 'percent' type of children's size (Layout Size minus spacings and parent padding)
	//                \
	//                  Content area size (Layout Size minus parent padding and unused space)
	//
	// Pixel   - Excludes the child margin
	// Percent - Uses the space, Includes the child margin
	// Ratio   - ? the child margin
	// Dynamic - Excludes the child margin

	// --- Build entries ---

	boost::container::small_vector<LayoutEntry, 32> entries;
	build_entries(entries, children, orientData._X_, orientData._Y_, orientData.inset_frontX, orientData.inset_backX, orientData.inset_bottomY);

	// --- Size ---

	// Margin / Spacing

	const auto layout_sizeX = layout_env.size[orientData._X_];
	const auto layout_sizeY = layout_env.size[orientData._Y_];
	const auto padding_sizeX = padding_size()[orientData._X_];
	const auto padding_sizeY = padding_size()[orientData._Y_];
	const auto padding_size = libv::vec2f{padding_sizeX, padding_sizeY};
	const auto spacing_sumX = entries.empty() ? 0.f : property.spacing() * static_cast<float>(entries.size() - 1);

	auto children_sum_marginX = 0.f;
	auto children_sum_ratioX = 0.f;
	auto unused_spaceX = layout_sizeX - padding_sizeX - spacing_sumX;

	// Pixel, Percent / Pre process pass
	const auto spaceDistributedByPercentX = layout_sizeX - padding_sizeX - spacing_sumX;
	const auto spaceDistributedByPercentY = layout_sizeY - padding_sizeY;

	for (auto& entry : entries) {
		children_sum_marginX += entry.marginSizeSW.x;
		children_sum_ratioX += entry.propertySizeSW.x.ratio;

		entry.resultSizeSW.x += entry.propertySizeSW.x.pixel;
		entry.resultSizeSW.y += entry.propertySizeSW.y.pixel;
		entry.resultSizeSW.x += entry.propertySizeSW.x.percent * 0.01f * spaceDistributedByPercentX;
		entry.resultSizeSW.y += entry.propertySizeSW.y.percent * 0.01f * spaceDistributedByPercentY;

		unused_spaceX -= entry.resultSizeSW.x;
	}

	unused_spaceX -= children_sum_marginX;

	// Dynamic
	for (auto& entry : entries) {
		const auto max_child_size = libv::vec2f{unused_spaceX, layout_sizeY - padding_sizeY - entry.marginSizeSW.y};
		const auto dynamic_contrib_size = swizzle(calculate_layout1_contrib(*entry.child, unswizzle(max_child_size, orientData._X_, orientData._Y_)), orientData._X_, orientData._Y_);
		entry.resultSizeSW += dynamic_contrib_size;

		unused_spaceX -= dynamic_contrib_size.x;
	}

	// Ratio / Post process pass
	auto maxUsedY = 0.f;

	const auto spaceDistributedByRatioX = unused_spaceX;
	const auto spaceDistributedByRatioScalerX = children_sum_ratioX > 0.f ? spaceDistributedByRatioX / children_sum_ratioX : 0.f;;
	if (children_sum_ratioX > 0.f)
		unused_spaceX = 0.f;

	for (auto& entry : entries) {
		entry.resultSizeSW.x += entry.propertySizeSW.x.ratio * spaceDistributedByRatioScalerX;

		if (entry.propertySizeSW.y.ratio > 0.f)
			entry.resultSizeSW.y = layout_sizeY - padding_sizeY - entry.marginSizeSW.y;

		const auto usedY = padding_sizeY + entry.resultSizeSW.y + entry.marginSizeSW.y;
		maxUsedY = std::max(maxUsedY, usedY);
	}

	// --- Position ---

	// Justify
	const auto isJustified = entries.size() >= 2 && (orientData._X_ == 0 ? alignHData.justified : alignVData.justified);
	const auto spaceDistributedForJustifyX = unused_spaceX;
	const auto justifyGapX = isJustified ? spaceDistributedForJustifyX / static_cast<float>(entries.size() - 1) : 0.f;
	if (isJustified)
		unused_spaceX = 0.f;

	// Alignment
	const auto alignmentScaleX = orientData._X_ == 0 ? alignHData.scale : alignVData.scale;
	const auto alignmentScaleY = orientData._Y_ == 0 ? alignHData.scale : alignVData.scale;
	const auto alignmentScale = libv::vec2f{alignmentScaleX, alignmentScaleY};

	// Assign
	const auto unused_spaceY = layout_sizeY - maxUsedY;
	const auto unused_space = libv::vec2f{unused_spaceX, unused_spaceY};
	const auto used_space = swizzle(layout_env.size, orientData._X_, orientData._Y_) - unused_space;

	const auto originToContent = unused_space * alignmentScale;
	const auto contentToStart = (used_space - padding_size) * swizzle(orientData.start, orientData._X_, orientData._Y_);

	auto startToPen = vec2f{};

	for (auto& entry : entries) {
		startToPen.x += orientData.direction[orientData._X_] * entry.marginFrontX;

		auto penToBase = entry.resultSizeSW * swizzle(orientData.penCorner, orientData._X_, orientData._Y_);
		penToBase.y += entry.marginSizeSW.y * orientData.penCorner[orientData._Y_];

		const auto unusedSpaceAboveChildY = used_space.y - padding_sizeY - entry.resultSizeSW.y - entry.marginSizeSW.y;
		auto baseToPosition = vec2f{};
		baseToPosition.y = unusedSpaceAboveChildY * alignmentScale.y;
		baseToPosition.y += entry.marginBottomY;

		const auto position = swizzle(padding_LB(), orientData._X_, orientData._Y_)
				+ originToContent
				+ contentToStart
				+ startToPen
				+ penToBase
				+ baseToPosition;

		const auto roundedPositionSW = libv::vec::round(position);
		const auto roundedPosition = unswizzle(roundedPositionSW, orientData._X_, orientData._Y_);
		const auto roundedSizeSW = libv::vec::round(position + entry.resultSizeSW) - roundedPositionSW;
		const auto roundedSize = unswizzle(roundedSizeSW, orientData._X_, orientData._Y_);

//		std::cout << "---------: " << entry.child->path() << std::endl;
//		std::cout << "layout_env.size: " << layout_env.size << std::endl;
//		std::cout << "unused_space: " << unused_space << std::endl;
//		std::cout << "used_space: " << used_space << std::endl;
//		std::cout << std::endl;
//		std::cout << "alignmentScale: " << alignmentScale << std::endl;
//		std::cout << "unusedSpaceAboveChildY: " << unusedSpaceAboveChildY << std::endl;
//		std::cout << std::endl;
//		std::cout << "isJustified: " << isJustified << std::endl;
//		std::cout << "spaceDistributedForJustifyX: " << spaceDistributedForJustifyX << std::endl;
//		std::cout << "justifyGapX: " << justifyGapX << std::endl;
//		std::cout << std::endl;
//		std::cout << "padding_sizeX: " << padding_sizeX << std::endl;
//		std::cout << "padding_sizeY: " << padding_sizeY << std::endl;
//		std::cout << std::endl;
//		std::cout << "entry.child->size(): " << entry.child->size() << std::endl;
//		std::cout << "entry.child->margin(): " << entry.child->margin() << std::endl;
//		std::cout << "entry.resultSizeSW: " << entry.resultSizeSW << std::endl;
//		std::cout << std::endl;
//		std::cout << "position = : " << position << std::endl;
//		std::cout << "    + padding_LB3(): " << swizzle(padding_LB(), orientData._X_, orientData._Y_) << std::endl;
//		std::cout << "    + originToContent: " << originToContent << std::endl;
//		std::cout << "    + contentToStart: " << contentToStart << std::endl;
//		std::cout << "    + startToPen: " << startToPen << std::endl;
//		std::cout << "    + penToBase: " << penToBase << std::endl;
//		std::cout << "    + baseToPosition: " << baseToPosition << std::endl;
//		std::cout << std::endl;
//		std::cout << "roundedPositionSW: " << roundedPositionSW << std::endl;
//		std::cout << "roundedSizeSW    : " << roundedSizeSW << std::endl;
//		std::cout << "roundedPosition  : " << roundedPosition << std::endl;
//		std::cout << "roundedSize      : " << roundedSize << std::endl;
//		std::cout << std::endl;

		AccessLayout::layout2(
				*entry.child,
				layout_env.enter(libv::vec3f{roundedPosition, 0.f}, libv::vec3f{roundedSize, 0.f})
		);

		startToPen.x += (entry.resultSizeSW.x + entry.marginBackX + property.spacing() + justifyGapX) * orientData.direction[orientData._X_];
	}
}

// =================================================================================================

core_ptr PanelLine::create_core(std::string name) {
	return create_core_ptr<CorePanelLine>(std::move(name));
}

// -------------------------------------------------------------------------------------------------

void PanelLine::align_horizontal(AlignHorizontal value) {
	AccessProperty::manual(self(), self().property.align_horizontal, value);
}

AlignHorizontal PanelLine::align_horizontal() const noexcept {
	return self().property.align_horizontal();
}

void PanelLine::align_vertical(AlignVertical value) {
	AccessProperty::manual(self(), self().property.align_vertical, value);
}

AlignVertical PanelLine::align_vertical() const noexcept {
	return self().property.align_vertical();
}

void PanelLine::orientation(Orientation value) {
	AccessProperty::manual(self(), self().property.orientation, value);
}

Orientation PanelLine::orientation() const noexcept {
	return self().property.orientation();
}

void PanelLine::spacing(Spacing value) {
	AccessProperty::manual(self(), self().property.spacing, value);
}

Spacing PanelLine::spacing() const noexcept {
	return self().property.spacing();
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
