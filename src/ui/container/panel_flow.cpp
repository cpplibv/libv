// File: panel_flow.cpp, Created on 2016.01.22. 14:32, Author: Vader

// hpp
#include <libv/ui/container/panel_flow.hpp>
// libv
#include <libv/adaptor.hpp>
#include <libv/memory.hpp>
#include <libv/vec.hpp>
// std
#include <algorithm>
// pro
#include <libv/ui/layout.hpp>
#include <libv/ui/log.hpp>
#include <libv/ui/properties_eval.hpp>

namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

namespace {

using Align = vec3;
struct Orient {
	size_t primaryDimension;
	size_t secondaryDimension;
	bool invertedPrimaryExpand;
	bool invertedSecondaryExpand;
};

static const Align alignmentTable[] = {
	{0.5f, 0.0f, 0.f}, //0 BOTTOM_CENTER
	{0.0f, 0.0f, 0.f}, //1 BOTTOM_LEFT
	{1.0f, 0.0f, 0.f}, //2 BOTTOM_RIGHT
	{0.5f, 0.5f, 0.f}, //3 CENTER_CENTER
	{0.0f, 0.5f, 0.f}, //4 CENTER_LEFT
	{1.0f, 0.5f, 0.f}, //5 CENTER_RIGHT
	{0.5f, 1.0f, 0.f}, //6 TOP_CENTER
	{0.0f, 1.0f, 0.f}, //7 TOP_LEFT
	{1.0f, 1.0f, 0.f}, //8 TOP_RIGHT
};

static const Orient orienationTable[] = {
	{1, 0, 0, 1}, //0 UP_LEFT
	{1, 0, 0, 0}, //1 UP_RIGHT
	{1, 0, 1, 1}, //2 DOWN_LEFT
	{1, 0, 1, 0}, //3 DOWN_RIGHT
	{0, 1, 1, 0}, //4 LEFT_UP
	{0, 1, 1, 1}, //5 LEFT_DOWN
	{0, 1, 0, 0}, //6 RIGHT_UP
	{0, 1, 0, 1}, //7 RIGHT_DOWN
};

// -------------------------------------------------------------------------------------------------

struct Line {
	std::vector<observer_ptr<Container::ContainedComponent>> compoments;
	vec3 size;
};

struct BuiltLines {
	std::vector<Line> lines;
	vec3 size;
};

BuiltLines buildLines(Container::Store& components, vec3 parentSize, const Orient& orient) {
	const auto dim1 = orient.primaryDimension;
	const auto dim2 = orient.secondaryDimension;

	std::vector<Line> lines;
	lines.emplace_back();
	size_t last = 0;
	vec3 sumSize;

	for (auto& comp : components) {
		const auto compSize = comp.info.size;

		if (!lines[last].compoments.empty() && lines[last].size[dim1] + compSize[dim1] > parentSize[dim1]) {
			lines.emplace_back();
			++last;
		}

		lines[last].compoments.emplace_back(&comp);
		lines[last].size[dim1] += compSize[dim1];
		lines[last].size[dim2] = std::max(lines[last].size[dim2], compSize[dim2]);
	}

	for (auto& line : lines) {
		sumSize[dim1] = std::max(sumSize[dim1], line.size[dim1]);
		sumSize[dim2] += line.size[dim2];
	}

	return BuiltLines{std::move(lines), sumSize};
}

} //namespace

// PanelFlow =======================================================================================

LayoutInfo PanelFlow::doLayout(const LayoutInfo& parentLayoutInfo) {
	const auto sizeContainer = evalLayoutSize(parentLayoutInfo, *this);
	LayoutInfo layout(sizeContainer);

	for (auto& component : components) {
		const auto childLayout = component.ptr->layout(layout);
		component.info.size = childLayout.size;
	}

	const auto indexAlign = getOrUse(Property::Align, ALIGN_BOTTOM_LEFT);
	const auto indexAnchor = getOrUse(Property::Anchor, ALIGN_TOP_LEFT);
	const auto indexOrient = getOrUse(Property::Orient, ORIENT_RIGHT_DOWN);

	const auto& align = alignmentTable[indexAlign];
	const auto& contentAnchor = alignmentTable[indexAnchor];
	const auto& orient = orienationTable[indexOrient];

	const auto dim1 = orient.primaryDimension;
	const auto dim2 = orient.secondaryDimension;

	const auto builtLines = buildLines(components, sizeContainer, orient);
	const auto& lines = builtLines.lines;
	const auto& sizeContent = builtLines.size;

	auto origToContent = (sizeContainer - sizeContent) * contentAnchor;
	vec3 contentToLine;

	for (const auto& line : lines | reverse_if(orient.invertedSecondaryExpand)) {
		const auto sizeLine = line.size;
		vec3 lineToComponent;

		contentToLine[dim1] = (sizeContent[dim1] - sizeLine[dim1]) * align[dim1];
		const auto& lineComponents = line.compoments;

		for (auto& component : lineComponents | reverse_if(orient.invertedPrimaryExpand)) {
			const auto pen = origToContent + contentToLine + lineToComponent;
			const auto sizeComponent = component->info.size;
			vec3 baselineOffset;
			baselineOffset[dim2] = (sizeLine[dim2] - sizeComponent[dim2]) * align[dim2];

			component->info.offset = pen + baselineOffset;

			lineToComponent[dim1] += sizeComponent[dim1];
		}
		contentToLine[dim2] += sizeLine[dim2];
	}

	return layout;
}

} //namespace ui
} //namespace libv
