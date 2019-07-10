// File:   layout_grid.cpp Author: Vader Created on 01 August 2019, 03:58

// hpp
#include <libv/ui/layout/layout_line.hpp>
// ext
#include <boost/container/small_vector.hpp>
#include <range/v3/view/zip.hpp>
// libv
#include <libv/utility/approxing.hpp>
#include <libv/utility/enum.hpp>
#include <libv/utility/min_max.hpp>
// pro
#include <libv/ui/component_base.hpp>
#include <libv/ui/context_layout.hpp>
#include <libv/ui/layout/view_layouted.lpp>
#include <libv/ui/log.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

namespace {

struct OrientationData {
	uint32_t _X_;
	uint32_t _Y_;
	uint32_t _Z_;
	libv::vec3f start;
	libv::vec3f direction;
	libv::vec3f penCorner;
};

static constexpr OrientationData OrientationTable[] = {
	{1u, 0u, 2u, {0.f, 0.f, 0.f}, {+1.f, +1.f, +1.f}, {+0.f, +0.f, +0.f}}, // BOTTOM_TO_TOP;
	{0u, 1u, 2u, {0.f, 0.f, 0.f}, {+1.f, +1.f, +1.f}, {+0.f, +0.f, +0.f}}, // LEFT_TO_RIGHT;
	{0u, 1u, 2u, {1.f, 0.f, 0.f}, {-1.f, +1.f, +1.f}, {-1.f, +0.f, +0.f}}, // RIGHT_TO_LEFT;
	{1u, 0u, 2u, {0.f, 1.f, 0.f}, {+1.f, -1.f, +1.f}, {+0.f, -1.f, +0.f}}, // TOP_TO_BOTTOM;
};

struct AlignmentData {
	float scale;
	float justifyGap;
	bool justified;
};

static constexpr AlignmentData AlignmentTableH[] = {
	{0.0f, 0.0f, false}, // Left
	{0.5f, 0.0f, false}, // Center
	{1.0f, 0.0f, false}, // Right
	{0.0f, 1.0f, true}, // Justify
	{0.0f, 1.0f, true}, // JustifyAll
};

static constexpr AlignmentData AlignmentTableV[] = {
	{1.0f, 0.0f, false}, // Top
	{0.5f, 0.0f, false}, // Center
	{0.0f, 0.0f, false}, // Bottom
	{0.0f, 1.0f, true}, // Justify
	{0.0f, 1.0f, true}, // JustifyAll
};


} // namespace

// -------------------------------------------------------------------------------------------------

libv::vec3f LayoutLine::layout1(
		const ContextLayout1& environment,
		libv::span<Child> children,
		const Properties& properties,
		const ComponentBase& parent) {

	(void) environment;

	const auto& orientData = OrientationTable[libv::to_value(properties.orientation())];
	const auto _X_ = orientData._X_;
	const auto _Y_ = orientData._Y_;
	const auto _Z_ = orientData._Z_;

	auto result = libv::vec3f{};
	auto pixelX = 0.f;
	auto percentX = 0.f;
	auto contentX = 0.f;

	const auto resolvePercent = [](const float fix, const float percent, auto& component) {
		if (fix == libv::Approxing(0.f)) {
			return fix;
		} else if (percent == libv::Approxing(100.f)) {
			log_ui.warn("Invalid sum of size percent {} with fixed width of {} during layout of {}", percent, fix, component.path());
			return fix * 2.f;
		} else {
			return fix / (1.f - percent * 0.01f);
		}
	};

	for (const auto& child : children | view_layouted()) {
		AccessLayout::layout1(*child.ptr, ContextLayout1{});

		pixelX += child.properties.size()[_X_].pixel;
		percentX += child.properties.size()[_X_].percent;
		contentX += (child.properties.size()[_X_].content ? AccessLayout::lastContent(*child.ptr)[_X_] : 0.0f);

		result[_Y_] = libv::max(result[_Y_],
				resolvePercent(
						child.properties.size()[_Y_].pixel + (child.properties.size()[_Y_].content ? AccessLayout::lastContent(*child.ptr)[_Y_] : 0.0f),
						child.properties.size()[_Y_].percent, *child.ptr));

		result[_Z_] = libv::max(result[_Z_],
				resolvePercent(
						child.properties.size()[_Z_].pixel + (child.properties.size()[_Z_].content ? AccessLayout::lastContent(*child.ptr)[_Z_] : 0.0f),
						child.properties.size()[_Z_].percent, *child.ptr));
	}

	result[_X_] = resolvePercent(pixelX + contentX, percentX, parent);

	return result;
}

void LayoutLine::layout2(
		const ContextLayout2& environment,
		libv::span<Child> children,
		const Properties& properties,
		const ComponentBase& parent) {

	(void) parent;

	const auto& alignHData = AlignmentTableH[libv::to_value(properties.alignHorizontal())];
	const auto& alignVData = AlignmentTableV[libv::to_value(properties.alignVertical())];
	const auto& orientData = OrientationTable[libv::to_value(properties.orientation())];

	const auto _X_ = orientData._X_;
	const auto _Y_ = orientData._Y_;
	const auto _Z_ = orientData._Z_;

	// Size ---

	auto contentX = 0.f;
	auto sumRatioX = 0.f;

	// Note: This will allocate more size as children includes non-layouted components too, but that is rare and saves a pass
	boost::container::small_vector<libv::vec3f, 16> childSizes(children.size(), libv::vec3f{});

	for (const auto& [child, childSize] : ranges::view::zip(children | view_layouted(), childSizes)) {

		libv::meta::n_times_index<3>([&](auto&& index) {
			childSize[index] =
					child.properties.size()[index].pixel +
					child.properties.size()[index].percent * 0.01f * environment.size[index] +
					(child.properties.size()[index].content ? AccessLayout::lastContent(*child.ptr)[index] : 0.f);
		});

		sumRatioX += child.properties.size()[_X_].ratio;
		contentX += childSize[_X_];
	}

	auto leftoverX = environment.size[_X_] - contentX;
	auto ratioScaledX = sumRatioX > 0.f ? leftoverX / sumRatioX : 0.f;
	auto sizeContent = vec3f{};

	for (const auto& [child, childSize] : ranges::view::zip(children | view_layouted(), childSizes)) {

		childSize[_X_] += ratioScaledX * child.properties.size()[_X_].ratio;
		if (child.properties.size()[_Y_].ratio > 0.f) childSize[_Y_] = environment.size[_Y_];
		if (child.properties.size()[_Z_].ratio > 0.f) childSize[_Z_] = environment.size[_Z_];

		sizeContent[_X_] += childSize[_X_];
		sizeContent[_Y_] = std::max(sizeContent[_Y_], childSize[_Y_]);
		sizeContent[_Z_] = std::max(sizeContent[_Z_], childSize[_Z_]);
	}

	// Position ---

	auto environmentUnused = environment.size - sizeContent;
	const auto justifyGapX = alignHData.justifyGap * environmentUnused[_X_] / std::max(static_cast<float>(children.size()) - 1.0f, 1.0f);
	if (alignHData.justified)
		environmentUnused[_X_] = 0.0f;

	const auto aligmentScale = vec3f{alignHData.scale, alignVData.scale, 0.0f};

	const auto origin = environment.position;
	const auto originToContent = environmentUnused * aligmentScale;
	const auto contentToStart = sizeContent * orientData.start;

	auto startToPen = vec3f{};

	for (const auto& [child, childSize] : ranges::view::zip(children | view_layouted(), childSizes)) {

		const auto penToBase = childSize * orientData.penCorner;
		auto baseToPosition = vec3f{};
		baseToPosition[_Y_] = (sizeContent[_Y_] - childSize[_Y_]) * aligmentScale[_Y_];
		const auto position = origin + originToContent + contentToStart + startToPen + penToBase + baseToPosition;

		startToPen[_X_] += orientData.direction[_X_] * childSize[_X_] + justifyGapX;

		const auto roundedPosition = libv::vec::round(position);
		const auto roundedSize = libv::vec::round(position + childSize) - roundedPosition;

		AccessLayout::layout2(
				*child.ptr,
				ContextLayout2{
					roundedPosition,
					roundedSize,
					MouseOrder{libv::to_value(environment.mouseOrder) + 1}
				}
		);
	}
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
