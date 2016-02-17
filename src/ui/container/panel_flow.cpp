// File: panel_flow.cpp, Created on 2016.01.22. 14:32, Author: Vader

// hpp
#include <libv/ui/container/panel_flow.hpp>
// ext
#include <boost/range/adaptor/reversed.hpp>
#include <glm/gtc/matrix_transform.hpp>
// libv
#include <libv/memory.hpp>
#include <libv/vec.hpp>
// std
#include <algorithm>
// pro
#include <libv/ui/log.hpp>

// TODO P5: rename Align to Anchor?

namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

using Align = vec3;
struct Orient {
	vec3 primaryMask;
	vec3 secondaryMask;
	vec3 primaryExpand;
	bool invertedPrimaryExpand;
	vec3 secondaryExpand;
	bool invertedSecondaryExpand;
};

static const Align alignmentTable[] = {
	{0.5f, 0.0f, 0.f}, //BOTTOM_CENTER
	{0.0f, 0.0f, 0.f}, //BOTTOM_LEFT
	{1.0f, 0.0f, 0.f}, //BOTTOM_RIGHT
	{0.5f, 0.5f, 0.f}, //CENTER_CENTER
	{0.0f, 0.5f, 0.f}, //CENTER_LEFT
	{1.0f, 0.5f, 0.f}, //CENTER_RIGHT
	{0.5f, 1.0f, 0.f}, //TOP_CENTER
	{0.0f, 1.0f, 0.f}, //TOP_LEFT
	{1.0f, 1.0f, 0.f}, //TOP_RIGHT
};

static const Orient orienationTable[] = {
	{vec3(0, 1, 0), vec3(1, 0, 0), vec3(0, +1, 0), 0, vec3(-1, 0, 0), 1}, //UP_LEFT
	{vec3(0, 1, 0), vec3(1, 0, 0), vec3(0, +1, 0), 0, vec3(+1, 0, 0), 0}, //UP_RIGHT
	{vec3(0, 1, 0), vec3(1, 0, 0), vec3(0, -1, 0), 1, vec3(-1, 0, 0), 1}, //DOWN_LEFT
	{vec3(0, 1, 0), vec3(1, 0, 0), vec3(0, -1, 0), 1, vec3(+1, 0, 0), 0}, //DOWN_RIGHT
	{vec3(1, 0, 0), vec3(0, 1, 0), vec3(-1, 0, 0), 1, vec3(0, +1, 0), 0}, //LEFT_UP
	{vec3(1, 0, 0), vec3(0, 1, 0), vec3(-1, 0, 0), 1, vec3(0, -1, 0), 1}, //LEFT_DOWN
	{vec3(1, 0, 0), vec3(0, 1, 0), vec3(+1, 0, 0), 0, vec3(0, +1, 0), 0}, //RIGHT_UP
	{vec3(1, 0, 0), vec3(0, 1, 0), vec3(+1, 0, 0), 0, vec3(0, -1, 0), 1}, //RIGHT_DOWN
};

const PanelFlow::Align PanelFlow::ALIGN_BOTTOM_CENTER = 0;
const PanelFlow::Align PanelFlow::ALIGN_BOTTOM_LEFT = 1;
const PanelFlow::Align PanelFlow::ALIGN_BOTTOM_RIGHT = 2;
const PanelFlow::Align PanelFlow::ALIGN_CENTER_CENTER = 3;
const PanelFlow::Align PanelFlow::ALIGN_CENTER_LEFT = 4;
const PanelFlow::Align PanelFlow::ALIGN_CENTER_RIGHT = 5;
const PanelFlow::Align PanelFlow::ALIGN_TOP_CENTER = 6;
const PanelFlow::Align PanelFlow::ALIGN_TOP_LEFT = 7;
const PanelFlow::Align PanelFlow::ALIGN_TOP_RIGHT = 8;

const PanelFlow::Orient PanelFlow::ORIENT_UP_LEFT = 0;
const PanelFlow::Orient PanelFlow::ORIENT_UP_RIGHT = 1;
const PanelFlow::Orient PanelFlow::ORIENT_DOWN_LEFT = 2;
const PanelFlow::Orient PanelFlow::ORIENT_DOWN_RIGHT = 3;
const PanelFlow::Orient PanelFlow::ORIENT_LEFT_UP = 4;
const PanelFlow::Orient PanelFlow::ORIENT_LEFT_DOWN = 5;
const PanelFlow::Orient PanelFlow::ORIENT_RIGHT_UP = 6;
const PanelFlow::Orient PanelFlow::ORIENT_RIGHT_DOWN = 7;

// Line ============================================================================================
class Line {
private:
	std::vector<adaptive_ptr<Component>> compoments;
	vec3 size;
	const Orient* orientData;
public:

	Line(const Orient& orientation) : orientData(&orientation) { }
	Line(const Line&) = default;
	Line(Line&&) = default;
	Line& operator=(const Line&) = default;
	Line& operator=(Line&&) = default;

	inline void add(adaptive_ptr<Component> comp) {
		compoments.push_back(comp);
		auto compSize = comp->get(Property::Size);
		size = maxByDimensions(size, compSize) * orientData->secondaryMask +
				(size + compSize) * orientData->primaryMask;
	}

	inline vec3 getSize() const {
		return size;
	}

	const Orient& getOrientation() const {
		return *orientData;
	}

	inline std::vector<adaptive_ptr<Component>> getCompoments() const {
		return compoments;
	}

	static vec3 accumlateSize(const std::vector<Line>& lines) {
		vec3 result;
		for (auto line : lines) {
			result = maxByDimensions(result, line.getSize()) * line.getOrientation().primaryMask +
					(result + line.getSize()) * line.getOrientation().secondaryMask;
		}
		return result;
	}
};

std::vector<Line> buildLines(
		Container::Store::const_iterator begin,
		Container::Store::const_iterator end,
		vec3 containerSize,
		const Orient& orientData) {

	std::vector<Line> lines;
	auto numComp = std::distance(begin, end);
	int i = 0;
	while (i < numComp) {
		Line currentLine(orientData);
		do {
			currentLine.add(*(begin + i));
			i++;
		} while (i < numComp &&
				containerSize * orientData.primaryMask >=
				(currentLine.getSize() + (*(begin + i))->get(Property::Size)) * orientData.primaryMask);
		lines.push_back(currentLine);
	}
	return lines;
}

// PanelFlow =======================================================================================

PanelFlow::PanelFlow(Orient orient, Align align, Align alignContent) :
	align(align),
	alignContent(alignContent),
	orient(orient) { }

// -------------------------------------------------------------------------------------------------

void PanelFlow::setAlign(Align align) {
	this->align = align;
}

void PanelFlow::setAlignContent(Align alignContent) {
	this->alignContent = alignContent;
}

void PanelFlow::setOrient(Orient orient) {
	this->orient = orient;
}

// -------------------------------------------------------------------------------------------------

void PanelFlow::doBuild(Renderer&) {
	layoutImpl();
}

void PanelFlow::layoutImpl() {
	LIBV_UI_LAYOUT_TRACE("PanelFlow Layout");

	auto begin = components.begin();
	auto end = components.end();

	const auto& currentOrient = orienationTable[orient];
	const auto& currentAlign = alignmentTable[align];
	const auto& currentAlignContent = alignmentTable[alignContent];

	const auto dim1 = [&](auto v) {
		return v * currentOrient.primaryMask;
	};
	const auto dim2 = [&](auto v) {
		return v * currentOrient.secondaryMask;
	};

	auto sizeContainer = getDisplaySize();
	auto lines = buildLines(begin, end, sizeContainer, currentOrient);
	auto sizeContent = Line::accumlateSize(lines);

	auto penContent = (sizeContainer - sizeContent) * currentAlignContent;
	vec3 penLine;

	const auto lineIndexFrom = currentOrient.invertedSecondaryExpand ? lines.size() - 1 : 0;
	const auto lineIndexTo = currentOrient.invertedSecondaryExpand ? -1 : lines.size();
	const auto lineIndexStep = currentOrient.invertedSecondaryExpand ? -1 : 1;

	for (auto i = lineIndexFrom; i != lineIndexTo; i += lineIndexStep) {
		const auto& line = lines[i];
		const auto sizeLine = line.getSize();

		vec3 penComponent;
		penLine = dim1((sizeContent - sizeLine) * currentAlign) + dim2(penLine);
		auto lineComponents = line.getCompoments();

		if (currentOrient.invertedPrimaryExpand)
			std::reverse(lineComponents.begin(), lineComponents.end());

		for (auto comp : lineComponents) {
			const auto pen = penLine + penContent + penComponent;
			const auto sizeComponent = comp->get(Property::Size);
			const auto baselineOffset = dim2((sizeLine - sizeComponent) * currentAlign);

			comp->setDisplayPosition(pen + baselineOffset);
			comp->setDisplaySize(sizeComponent);

			penComponent += dim1(sizeComponent);
		}
		penLine += dim2(sizeLine);
	}
}

} //namespace ui
} //namespace libv
