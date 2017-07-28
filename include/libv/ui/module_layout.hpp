// File: layout.hpp - Created on 2017.10.19. 01:21 - Author: Vader

#pragma once

// libv
#include <libv/math/vec.hpp>
// std
#include <cstdint>
// pro
#include <libv/ui/module.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class ComponentBase;
class ContextLayoutPass1;
class ContextLayoutPass2;

// -------------------------------------------------------------------------------------------------

struct ModuleLayout : Module {
	virtual void layoutPass1(const ContextLayoutPass1& environment, ComponentBase& component) = 0;
	virtual void layoutPass2(const ContextLayoutPass2& environment, ComponentBase& component) = 0;
	virtual ~ModuleLayout() = default;
};

// -------------------------------------------------------------------------------------------------

class Alignment {
	uint32_t index;

public:
	constexpr inline Alignment(uint32_t index) : index(index) { }

	struct AlignmentData {
		float scale;
		float justifyGap;
		bool justified;
	};

	static constexpr uint32_t LEFT = 0;
	static constexpr uint32_t BOTTOM = LEFT;
	static constexpr uint32_t CENTER = 1;
	static constexpr uint32_t RIGHT = 2;
	static constexpr uint32_t TOP = RIGHT;
	static constexpr uint32_t JUSTIFY = 3;
	static constexpr uint32_t BASELINE = 4; /// Not implemented yet.

	const AlignmentData& get() {
		static constexpr AlignmentData table[] = {
			{0.0f, 0.0f, false}, // LEFT, BOTTOM
			{0.5f, 0.0f, false}, // CENTER
			{1.0f, 0.0f, false}, // RIGHT, TOP
			{0.0f, 1.0f, true}, // JUSTIFY;
			{0.0f, 0.0f, false}, // BASELINE;
		};
		return table[index];
	}
};

class Orientation {
	uint32_t index;

public:
	constexpr inline Orientation(uint32_t index) : index(index) { }

	struct OrientationData {
		uint32_t dim1;
		uint32_t dim2;
		uint32_t dim3;
		libv::vec3f start;
		libv::vec3f direction;
		libv::vec3f penCorner;
	};

	static constexpr uint32_t BOTTOM_TO_TOP = 0;
	static constexpr uint32_t LEFT_TO_RIGHT = 1;
	static constexpr uint32_t RIGHT_TO_LEFT = 2;
	static constexpr uint32_t TOP_TO_BOTTOM = 3;

	const OrientationData& get() {
		static constexpr OrientationData table[] = {
			{1u, 0u, 2u, {0.f, 0.f, 0.f}, {+1.f, +1.f, +1.f}, {+0.f, +0.f, +0.f}}, // BOTTOM_TO_TOP;
			{0u, 1u, 2u, {0.f, 0.f, 0.f}, {+1.f, +1.f, +1.f}, {+0.f, +0.f, +0.f}}, // LEFT_TO_RIGHT;
			{0u, 1u, 2u, {1.f, 0.f, 0.f}, {-1.f, +1.f, +1.f}, {-1.f, +0.f, +0.f}}, // RIGHT_TO_LEFT;
			{1u, 0u, 2u, {0.f, 1.f, 0.f}, {+1.f, -1.f, +1.f}, {+0.f, -1.f, +0.f}}, // TOP_TO_BOTTOM;
		};
		return table[index];
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
