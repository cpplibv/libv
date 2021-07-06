// Project: libv.ui, File: src/libv/ui/component/panel_line.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <libv/ui/component/panel_line.hpp>
#include <libv/ui/component/panel_line_core.hpp>
// ext
#include <boost/container/small_vector.hpp>
#include <range/v3/view/zip.hpp>
// libv
#include <libv/algo/adjacent_pairs.hpp>
#include <libv/utility/approx.hpp>
#include <libv/utility/min_max.hpp>
#include <libv/utility/to_underlying.hpp>
// pro
#include <libv/ui/component/detail/core_component.hpp>
#include <libv/ui/component/base_panel.lpp>
#include <libv/ui/context/context_layout.hpp>
#include <libv/ui/context/context_style.hpp>
#include <libv/ui/layout/view_layouted.lpp>
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

static_assert(libv::to_underlying(Orientation::BOTTOM_TO_TOP) == 0u);
static_assert(libv::to_underlying(Orientation::LEFT_TO_RIGHT) == 1u);
static_assert(libv::to_underlying(Orientation::RIGHT_TO_LEFT) == 2u);
static_assert(libv::to_underlying(Orientation::TOP_TO_BOTTOM) == 3u);
// Inset: 0 = left, 1 = bottom, 2 = right, 3 = top
static constexpr OrientationData OrientationTable[] = {
	{1u, 0u, 2u, 1u, 3u, 2u, 0u, {0.f, 0.f, 0.f}, {+1.f, +1.f, +1.f}, {+0.f, +0.f, +0.f}}, // BOTTOM_TO_TOP;
	{0u, 1u, 2u, 0u, 2u, 3u, 1u, {0.f, 0.f, 0.f}, {+1.f, +1.f, +1.f}, {+0.f, +0.f, +0.f}}, // LEFT_TO_RIGHT;
	{0u, 1u, 2u, 2u, 0u, 3u, 1u, {1.f, 0.f, 0.f}, {-1.f, +1.f, +1.f}, {-1.f, +0.f, +0.f}}, // RIGHT_TO_LEFT;
	{1u, 0u, 2u, 3u, 1u, 2u, 0u, {0.f, 1.f, 0.f}, {+1.f, -1.f, +1.f}, {+0.f, -1.f, +0.f}}, // TOP_TO_BOTTOM;
};

struct AlignmentData {
	float scale;
	bool justified;
};

static constexpr AlignmentData AlignmentTableH[] = {
	{0.0f, false}, // Left
	{0.5f, false}, // Center
	{1.0f, false}, // Right
	{0.0f, true}, // Justify
	{0.0f, true}, // JustifyAll
};

static constexpr AlignmentData AlignmentTableV[] = {
	{1.0f, false}, // Top
	{0.5f, false}, // Center
	{0.0f, false}, // Bottom
	{0.0f, true}, // Justify
	{0.0f, true}, // JustifyAll
};

} // namespace

// -------------------------------------------------------------------------------------------------

void CorePanelLine::doStyle(ContextStyle& ctx) {
	PropertyAccessContext<Properties> setter{property, ctx.component, ctx.style, ctx.component.context()};
	access_properties(setter);
	CoreComponent::access_properties(setter);
}

void CorePanelLine::doStyle(ContextStyle& ctx, ChildID childID) {
	(void) ctx;
	(void) childID;
}

// -------------------------------------------------------------------------------------------------

libv::vec3f CorePanelLine::doLayout1(const ContextLayout1& layout_env) {
	const auto& orientData = OrientationTable[libv::to_value(property.orientation())];
	const auto _X_ = orientData._X_;
	const auto _Y_ = orientData._Y_;
	const auto _Z_ = orientData._Z_;

	const auto resolvePercent = [](const float fix, const float percent, auto& component) {
		if (fix == libv::Approx(0.f)) {
			return fix;
		} else if (percent == libv::Approx(100.f)) {
			log_ui.warn("Invalid sum of size percent {} with fixed width of {} during layout of {}", percent, fix, component.path());
			return fix * 2.f;
		} else {
			return fix / (1.f - percent * 0.01f);
		}
	};

	// --- SpacingX ---

	auto margin_spacing_sumX = 0.f;

	for (const Component& child : children) {
		if (child.core().isLayouted()) {
			// First child only
			margin_spacing_sumX += child.margin()[orientData.inset_frontX];
			break;
		}
	}

	for (const Component& child : children | std::views::reverse) {
		if (child.core().isLayouted()) {
			// Last child only
			margin_spacing_sumX += child.margin()[orientData.inset_backX];
			break;
		}
	}

	libv::algo::adjacent_pairs(children | view_layouted(), [&](const Component& a, const Component& b) {
		const auto a_back = a.margin()[orientData.inset_backX];
		const auto b_front = b.margin()[orientData.inset_frontX];
		margin_spacing_sumX += std::max(property.spacing(), std::max(a_back, b_front));
	});

	libv::vec3f margin_spacing_sum;
	margin_spacing_sum[_X_] = margin_spacing_sumX;

	// --- Size pass 1 ---

	const auto env_size = layout_env.size
			- margin_spacing_sum
			- padding_size3();

	auto result = libv::vec3f{};
	auto pixelX = 0.f;
	auto percentX = 0.f;
	auto contentX = 0.f;

	for (auto& child : children | view_layouted()) {
		auto child_env_size = env_size;
		child_env_size[_Y_] -= child.padding_size3()[_Y_];
		const auto child_dynamic = child.size().has_dynamic() ?
				AccessLayout::layout1(child.core(), ContextLayout1{child_env_size}) :
				libv::vec3f{};

		pixelX += child.size()[_X_].pixel;
		percentX += child.size()[_X_].percent;
		contentX += (child.size()[_X_].dynamic ? child_dynamic[_X_] : 0.0f);

		result[_Y_] = libv::max(result[_Y_],
				resolvePercent(
						child.size()[_Y_].pixel + (child.size()[_Y_].dynamic ? child_dynamic[_Y_] : 0.0f),
						child.size()[_Y_].percent, child)
					+ std::max(0.f, child.margin()[orientData.inset_topY] - padding()[orientData.inset_topY])
					+ std::max(0.f, child.margin()[orientData.inset_bottomY] - padding()[orientData.inset_bottomY]));

		result[_Z_] = libv::max(result[_Z_],
				resolvePercent(
						child.size()[_Z_].pixel + (child.size()[_Z_].dynamic ? child_dynamic[_Z_] : 0.0f),
						child.size()[_Z_].percent, child));
	}

	result[_X_] = resolvePercent(pixelX + contentX, percentX, *this);

	// --- Sum ---
	return result
			+ margin_spacing_sum
			+ padding_size3();
}

void CorePanelLine::doLayout2(const ContextLayout2& layout_env) {
	const auto& alignHData = AlignmentTableH[property.align_horizontal()];
	const auto& alignVData = AlignmentTableV[property.align_vertical()];
	const auto& orientData = OrientationTable[libv::to_value(property.orientation())];
	const auto _X_ = orientData._X_;
	const auto _Y_ = orientData._Y_;
	const auto _Z_ = orientData._Z_;

	// --- Build entries ---

	struct Entry {
		CoreComponent* child = nullptr;
		libv::vec3f size;
		float front_spacingX;
//		float jastifyGapRatio;
	};

	boost::container::small_vector<Entry, 32> entries;
	entries.resize(children.size()); // Note: This resize may allocate more memory as children may include non-layouted components too, but that is rare and saves a pass
	int entries_num = 0;
	for (auto& c : children | view_layouted())
		entries[entries_num++].child = &c.core();
	entries.resize(entries_num);

	// --- SpacingX ---

	auto margin_spacing_sumX = 0.f;

	if (!entries.empty()) { // First child only
		Entry& b = entries.front();
		b.front_spacingX = std::max(0.f, b.child->margin()[orientData.inset_frontX] - padding()[orientData.inset_frontX]);
		margin_spacing_sumX += b.front_spacingX;
	}

	if (!entries.empty()) { // Last child only
		Entry& a = entries.back();
		margin_spacing_sumX += a.child->margin()[orientData.inset_backX];
	}

	libv::algo::adjacent_pairs(entries, [&](Entry& a, Entry& b) {
		const auto a_back = a.child->margin()[orientData.inset_backX];
		const auto b_front = b.child->margin()[orientData.inset_frontX];
		b.front_spacingX = std::max(property.spacing(), std::max(a_back, b_front));
		margin_spacing_sumX += b.front_spacingX;
	});

	libv::vec3f margin_spacing_sum;
	margin_spacing_sum[_X_] = margin_spacing_sumX;

	// --- Size ---

	const auto percent_size = layout_env.size - padding_size3();
	const auto env_size = layout_env.size - margin_spacing_sum - padding_size3();

	auto contentX = 0.f;
	auto sumRatioX = 0.f;

	for (auto& entry : entries) {
		auto& child = *entry.child;
		auto& childSize = entry.size;

		auto child_env_size = env_size;
		child_env_size[_Y_] -= child.padding_size3()[_Y_];
		const auto child_dynamic = child.size().has_dynamic() ?
				AccessLayout::layout1(child, ContextLayout1{child_env_size}) :
				libv::vec3f{};

		libv::meta::for_constexpr<0, 3>([&](auto&& i) {
			childSize[i] =
					child.size()[i].pixel +
					child.size()[i].percent * 0.01f * percent_size[i] +
					(child.size()[i].dynamic ? child_dynamic[i] : 0.f);
		});

		sumRatioX += child.size()[_X_].ratio;
		contentX += childSize[_X_];
	}

	auto leftoverX = env_size[_X_] - contentX;
	auto ratioScalerX = sumRatioX > 0.f ? leftoverX / sumRatioX : 0.f;
	auto sizeContent = vec3f{};

	for (auto& entry : entries) {
		const auto& child = *entry.child;
		auto& childSize = entry.size;

		childSize[_X_] += ratioScalerX * child.size()[_X_].ratio;
		if (child.size()[_Y_].ratio > 0.f)
			childSize[_Y_] = env_size[_Y_]
//					- child.margin()[orientData.inset_topY]
//					- child.margin()[orientData.inset_bottomY];
					- std::max(0.f, child.margin()[orientData.inset_topY] - padding()[orientData.inset_topY])
					- std::max(0.f, child.margin()[orientData.inset_bottomY] - padding()[orientData.inset_bottomY]);
		if (child.size()[_Z_].ratio > 0.f)
			childSize[_Z_] = env_size[_Z_];

		sizeContent[_X_] += childSize[_X_];
		sizeContent[_Y_] = std::max(sizeContent[_Y_], childSize[_Y_]);
		sizeContent[_Z_] = std::max(sizeContent[_Z_], childSize[_Z_]);
	}

	sizeContent[_X_] += margin_spacing_sumX;

	// --- SpacingX Justify distribution ---

	auto environmentUnused = layout_env.size - sizeContent;

	if (alignHData.justified && entries.size() >= 2) {
		// Gather GAPS
		// Sort GAPS by size
		// Calculate new value for the lowest N GAP
		// Assign the new value to lowest N GAP

		boost::container::small_vector<Entry*, 32> gaps(entries_num - 1, nullptr);
		for (size_t i = 1; i < entries.size(); ++i) // Skip first entry
			gaps[i - 1] = &entries[i];

		std::ranges::sort(gaps, std::less<>{}, [](Entry* e) { return e->front_spacingX; });

		auto unused_gap_size_sum = environmentUnused[_X_];
		auto lowest_gap_size = gaps[0]->front_spacingX;

		size_t i = 1;
		for (; i < gaps.size(); ++i) {
			const auto i_f = static_cast<float>(i);

			const auto curr_gap_size = gaps[i - 1]->front_spacingX;
			const auto next_gap_size = gaps[i - 0]->front_spacingX;

			const auto diff_to_next = next_gap_size - curr_gap_size;
			const auto diff_to_next_times_i = (i_f) * diff_to_next;

			if (diff_to_next_times_i < unused_gap_size_sum) {
				lowest_gap_size += diff_to_next;
				unused_gap_size_sum -= diff_to_next_times_i;
			} else {
				break;
			}
		}

		lowest_gap_size += unused_gap_size_sum / static_cast<float>(i);

		for (size_t j = 0; j < i; ++j)
			gaps[j]->front_spacingX = lowest_gap_size;

		sizeContent[_X_] += environmentUnused[_X_];
		environmentUnused[_X_] = 0.0f;
	}

	// --- Position ---

	const auto alignmentScale = vec3f{alignHData.scale, alignVData.scale, 0.0f};

	const auto originToContent = (environmentUnused - padding_size3()) * alignmentScale;
	const auto contentToStart = sizeContent * orientData.start;

	auto startToPen = vec3f{};

	for (auto& entry : entries) {
		auto& child = *entry.child;
		auto& childSize = entry.size;

		startToPen[_X_] += orientData.direction[_X_] * entry.front_spacingX;

		auto penToBase = childSize * orientData.penCorner;
		penToBase[_Y_] += child.margin_size3()[_Y_] * orientData.penCorner[_Y_];

		const auto spaceY_to_border_top = padding()[orientData.inset_topY] + (env_size[_Y_] - childSize[_Y_]) * (1.f - alignmentScale[_Y_]);
		const auto spaceY_to_border_bottom = padding()[orientData.inset_bottomY] + (env_size[_Y_] - childSize[_Y_]) * alignmentScale[_Y_];
		const auto marginY_needs_more_top = std::max(0.f, child.margin()[orientData.inset_topY] - spaceY_to_border_top);
		const auto marginY_needs_more_bottom = std::max(0.f, child.margin()[orientData.inset_bottomY] - spaceY_to_border_bottom);

		if (marginY_needs_more_bottom > 0.f && marginY_needs_more_top > 0.f)
			log_ui.warn("Insufficient space for margins for {}. Requested additional space on both secondary side", child.path());

		auto baseToPosition = vec3f{};
		baseToPosition[_Y_] = (sizeContent[_Y_] - childSize[_Y_]) * alignmentScale[_Y_];
		baseToPosition[_Y_] += marginY_needs_more_bottom;
		baseToPosition[_Y_] -= marginY_needs_more_top;

		const auto position = padding_LB3()
				+ originToContent
				+ contentToStart
				+ startToPen
				+ penToBase
				+ baseToPosition;

		// std::cout << "---------: " << child.path() << std::endl;
		// std::cout << "env_size: " << env_size << std::endl;
		// std::cout << "sizeContent: " << sizeContent << std::endl;
		// std::cout << "padding(): " << padding() << std::endl;
		// std::cout << "childSize: " << childSize << std::endl;
		// std::cout << "child.margin(): " << child.margin() << std::endl;
		// std::cout << std::endl;
		// std::cout << "spaceY_to_border_bottom: " << spaceY_to_border_bottom << std::endl;
		// std::cout << "spaceY_to_border_top: " << spaceY_to_border_top << std::endl;
		// std::cout << "marginY_needs_more_bottom: " << marginY_needs_more_bottom << std::endl;
		// std::cout << "marginY_needs_more_top: " << marginY_needs_more_top << std::endl;
		// std::cout << std::endl;
		// std::cout << "position = : " << position << std::endl;
		// std::cout << "    + padding_LB3(): " << padding_LB3() << std::endl;
		// std::cout << "    + originToContent: " << originToContent << std::endl;
		// std::cout << "    + contentToStart: " << contentToStart << std::endl;
		// std::cout << "    + startToPen: " << startToPen << std::endl;
		// std::cout << "    + penToBase: " << penToBase << std::endl;
		// std::cout << "    + baseToPosition: " << baseToPosition << std::endl;
		// std::cout << std::endl;

		const auto roundedPosition = libv::vec::round(position);
		const auto roundedSize = libv::vec::round(position + childSize) - roundedPosition;

		AccessLayout::layout2(
				child,
				layout_env.enter(roundedPosition, roundedSize)
		);

		startToPen[_X_] += orientData.direction[_X_] * childSize[_X_];
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

Spacing PanelLine::spacing() const noexcept{
	return self().property.spacing();
}

// -------------------------------------------------------------------------------------------------

void PanelLine::add(Component component) {
	self().add(std::move(component));
}

void PanelLine::remove(Component& component) {
	self().remove(component);
}

void PanelLine::clear() {
	self().clear();
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
