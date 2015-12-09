// File: LayoutManager.cpp, Created on 2014. január 7. 15:23, Author: Vader

// vl
#include <vl/memory.hpp>
#include <vl/vec.hpp>
// std
#include <algorithm>
// pro
#include <vl/ui/layout/layout_flow.hpp>
#include <vl/ui/component/container.hpp>
#include <vl/ui/component/component.hpp>

namespace vl {
namespace ui {

//TODO P2: change vec based dimension indexing into index based (position[dimIndex])

struct LayoutFlow::OrienationImpl {
	ivec3 primaryMask;
	ivec3 secondaryMask;
	ivec3 primaryExpand;
	int invertedPrimaryExpand;
	ivec3 secondaryExpand;
	int invertedSecondaryExpand;
};
struct LayoutFlow::AlignmentImpl {
	vec3 data;
};

const LayoutFlow::OrienationImpl EXPAND_DATA[] = {
	{ivec3(0, 1, 0), ivec3(1, 0, 0), ivec3(0, +1, 0), 0, ivec3(-1, 0, 0), 1}, //UP_LEFT
	{ivec3(0, 1, 0), ivec3(1, 0, 0), ivec3(0, +1, 0), 0, ivec3(+1, 0, 0), 0}, //UP_RIGHT
	{ivec3(0, 1, 0), ivec3(1, 0, 0), ivec3(0, -1, 0), 1, ivec3(-1, 0, 0), 1}, //DOWN_LEFT
	{ivec3(0, 1, 0), ivec3(1, 0, 0), ivec3(0, -1, 0), 1, ivec3(+1, 0, 0), 0}, //DOWN_RIGHT
	{ivec3(1, 0, 0), ivec3(0, 1, 0), ivec3(-1, 0, 0), 1, ivec3(0, +1, 0), 0}, //LEFT_UP
	{ivec3(1, 0, 0), ivec3(0, 1, 0), ivec3(-1, 0, 0), 1, ivec3(0, -1, 0), 1}, //LEFT_DOWN
	{ivec3(1, 0, 0), ivec3(0, 1, 0), ivec3(+1, 0, 0), 0, ivec3(0, +1, 0), 0}, //RIGHT_UP
	{ivec3(1, 0, 0), ivec3(0, 1, 0), ivec3(+1, 0, 0), 0, ivec3(0, -1, 0), 1}, //RIGHT_DOWN
};
//const LayoutFlow::AlignmentImpl ALIGNMENT_DATA[] = {
//	//TODO P5: Cleanup a bit
//};

LayoutFlow::Orienation const LayoutFlow::ORIENTATION_UP_LEFT = &EXPAND_DATA[0];
LayoutFlow::Orienation const LayoutFlow::ORIENTATION_UP_RIGHT = &EXPAND_DATA[1];
LayoutFlow::Orienation const LayoutFlow::ORIENTATION_DOWN_LEFT = &EXPAND_DATA[2];
LayoutFlow::Orienation const LayoutFlow::ORIENTATION_DOWN_RIGHT = &EXPAND_DATA[3];
LayoutFlow::Orienation const LayoutFlow::ORIENTATION_LEFT_UP = &EXPAND_DATA[4];
LayoutFlow::Orienation const LayoutFlow::ORIENTATION_LEFT_DOWN = &EXPAND_DATA[5];
LayoutFlow::Orienation const LayoutFlow::ORIENTATION_RIGHT_UP = &EXPAND_DATA[6];
LayoutFlow::Orienation const LayoutFlow::ORIENTATION_RIGHT_DOWN = &EXPAND_DATA[7];

const LayoutFlow::AlignmentImpl ALIGNMENT_BOTTOM_CENTER_DATA = {vec3(0.5f, 0.0f, 0.f)};
const LayoutFlow::AlignmentImpl ALIGNMENT_BOTTOM_LEFT_DATA = {vec3(0.0f, 0.0f, 0.f)};
const LayoutFlow::AlignmentImpl ALIGNMENT_BOTTOM_RIGHT_DATA = {vec3(1.0f, 0.0f, 0.f)};
const LayoutFlow::AlignmentImpl ALIGNMENT_CENTER_CENTER_DATA = {vec3(0.5f, 0.5f, 0.f)};
const LayoutFlow::AlignmentImpl ALIGNMENT_CENTER_LEFT_DATA = {vec3(0.0f, 0.5f, 0.f)};
const LayoutFlow::AlignmentImpl ALIGNMENT_CENTER_RIGHT_DATA = {vec3(1.0f, 0.5f, 0.f)};
const LayoutFlow::AlignmentImpl ALIGNMENT_TOP_CENTER_DATA = {vec3(0.5f, 1.0f, 0.f)};
const LayoutFlow::AlignmentImpl ALIGNMENT_TOP_LEFT_DATA = {vec3(0.0f, 1.0f, 0.f)};
const LayoutFlow::AlignmentImpl ALIGNMENT_TOP_RIGHT_DATA = {vec3(1.0f, 1.0f, 0.f)};

LayoutFlow::Alignment const LayoutFlow::ALIGNMENT_BOTTOM_CENTER = &ALIGNMENT_BOTTOM_CENTER_DATA;
LayoutFlow::Alignment const LayoutFlow::ALIGNMENT_BOTTOM_LEFT = &ALIGNMENT_BOTTOM_LEFT_DATA;
LayoutFlow::Alignment const LayoutFlow::ALIGNMENT_BOTTOM_RIGHT = &ALIGNMENT_BOTTOM_RIGHT_DATA;
LayoutFlow::Alignment const LayoutFlow::ALIGNMENT_CENTER_CENTER = &ALIGNMENT_CENTER_CENTER_DATA;
LayoutFlow::Alignment const LayoutFlow::ALIGNMENT_CENTER_LEFT = &ALIGNMENT_CENTER_LEFT_DATA;
LayoutFlow::Alignment const LayoutFlow::ALIGNMENT_CENTER_RIGHT = &ALIGNMENT_CENTER_RIGHT_DATA;
LayoutFlow::Alignment const LayoutFlow::ALIGNMENT_TOP_CENTER = &ALIGNMENT_TOP_CENTER_DATA;
LayoutFlow::Alignment const LayoutFlow::ALIGNMENT_TOP_LEFT = &ALIGNMENT_TOP_LEFT_DATA;
LayoutFlow::Alignment const LayoutFlow::ALIGNMENT_TOP_RIGHT = &ALIGNMENT_TOP_RIGHT_DATA;

class Line {
private:
	std::vector<adaptive_ptr<Component>> compoments;
	ivec3 size;
	LayoutFlow::Orienation orientation;
public:

	Line(LayoutFlow::Orienation orientation) : orientation(orientation) { }

	inline void add(adaptive_ptr<Component> comp) {
		compoments.push_back(comp);
		auto compSize = comp->get(Property::Size);
		size = maxByComponents(size, compSize) * orientation->secondaryMask +
				(size + compSize) * orientation->primaryMask;
	}

	inline ivec3 getSize() const {
		return size;
	}

	LayoutFlow::Orienation getOrientation() const {
		return orientation;
	}

	inline std::vector<adaptive_ptr<Component>> getCompoments() const {
		return compoments;
	}

	static ivec3 accumlateSize(const std::vector<Line>& lines) {
		ivec3 result;
		for (auto line : lines) {
			//TODO P5: EXTRA large object may "brake" the orientation... look it up

			result = maxByComponents(result, line.getSize()) * line.getOrientation()->primaryMask +
					(result + line.getSize()) * line.getOrientation()->secondaryMask;
		}
		return result;
	}
};

std::vector<Line> buildLines(
		Container::const_iterator begin,
		Container::const_iterator end,
		ivec3 containerSize,
		LayoutFlow::Orienation orientation) {

	std::vector<Line> lines;
	auto numComp = begin - end;
	int i = 0;
	while (i < numComp) {
		Line currentLine(orientation);
		do {
			currentLine.add(*(begin + i));
			i++;
		} while (i < numComp &&
				containerSize * orientation->primaryMask >=
				(currentLine.getSize() + (*(begin + i))->get(Property::Size)) * orientation->primaryMask);
		lines.push_back(currentLine);
	}
	return lines;
}

// -------------------------------------------------------------------------------------------------

LayoutFlow::LayoutFlow(Orienation orientation, Alignment align, Alignment lineAlign) :
	orientation(orientation), align(align), lineAlign(lineAlign) { }

//ivec3 LayoutFlow::layoutSize(const ProtectedContainer* target) {
//	auto sizeContainer = target->get(Property::Size);
//	auto lines = buildLines(target->getComponents(), sizeContainer, orientation);
//
//	return Line::accumlateSize(lines);
//}

void LayoutFlow::layout(Container::iterator begin, Container::iterator end, Component* target) {
//void LayoutFlow::layoutContainer(ProtectedContainer* target) {
	auto sizeContainer = target->get(Property::Size);
	auto lines = buildLines(begin, end, sizeContainer, orientation);
	auto sizeContent = Line::accumlateSize(lines);

	auto cursorContent = ivec3((sizeContainer - sizeContent) * lineAlign->data);
	ivec3 cursorLine;
	if (orientation->invertedSecondaryExpand) std::reverse(lines.begin(), lines.end());
	for (auto line : lines) {
		cursorLine = ivec3(
				(sizeContent - line.getSize()) * align->data * orientation->primaryMask +
				cursorLine * orientation->secondaryMask);
		auto cursorComponent = ivec3(0, 0, 0);
		auto lineComponents = line.getCompoments();
		if (orientation->invertedPrimaryExpand) std::reverse(lineComponents.begin(), lineComponents.end());
		for (auto comp : lineComponents) {
			comp->setDisplayPosition(ivec3(
					(cursorContent + cursorLine + cursorComponent) +
					((line.getSize() - comp->get(Property::Size)) *
					align->data * orientation->secondaryMask)
					));
			comp->setDisplaySize(comp->get(Property::Size));
			cursorComponent += ivec3(comp->get(Property::Size) * orientation->primaryMask);
		}
		cursorLine += line.getSize() * orientation->secondaryMask;
	}
}

} //namespace ui
} //namespace vl