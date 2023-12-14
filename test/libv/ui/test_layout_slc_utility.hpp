// Project: libv.ui, File: test/libv/ui/test_layout_utility.hpp

#pragma once

// pro
#include "test_layout_utility.hpp"
#include <libv/ui/component/layout/layout_slc.hpp>


// -------------------------------------------------------------------------------------------------

using namespace libv::ui;

// -------------------------------------------------------------------------------------------------

template <typename T>
struct TestCorePlan {
	T copy;

	template <typename Access>
	void access_layout(Access&& access) {
		access(copy);
	}
};

template <typename T>
[[nodiscard]] libv::vec2f testQueryPass1(libv::vec2f limit, libv::vec2f viewport, const T& plan) {
	TestCorePlan<T> core{plan};

	const auto unlimited = libv::vec2b(limit.x < 0, limit.y < 0);
	return libv::ui::layoutSLCCorePass1(core, libv::ui::ContextLayout1{unlimited, limit, viewport});
}

template <typename T>
[[nodiscard]] libv::vec2f testQueryPass1(libv::vec2f size, const T& plan) {
	return testQueryPass1(size, size, plan);
}

template <typename T>
void testLayoutPass2(libv::vec2f size, T& plan) {
	TestCorePlan<T> core{plan};
	libv::ui::layoutSLCCorePass2(core, libv::ui::ContextLayout2{libv::vec2f{}, size});
}

struct TestComp {
	libv::ui::Size size;
	MarginExtent margin;
	PaddingExtent padding;

	libv::ui::Anchor anchor;
	libv::vec2f virtual_dynamic; // Value that simulates the result of a layout1 pass

	libv::vec2f layout_position;
	libv::vec2f layout_size;

	explicit TestComp(Size size = {}, MarginExtent margin = {}, PaddingExtent padding = {}, Anchor anchor = {}, libv::vec2f dynamic = {}) :
			size(std::move(size)),
			margin(margin),
			padding(padding),
			anchor(anchor),
			virtual_dynamic(dynamic) {}

	explicit TestComp(std::string_view size_str, MarginExtent margin = {}, PaddingExtent padding = {}, Anchor anchor = {}, libv::vec2f dynamic = {}) :
			TestComp(libv::ui::parse_size_or_throw(size_str), margin, padding, anchor, dynamic) {}

	[[nodiscard]] constexpr inline libv::vec4f bounds() const noexcept {
		return {layout_position, layout_size};
	}

	[[nodiscard]] constexpr inline libv::vec2f calculate_dynamic(libv::vec2b unlimited, libv::vec2f limits) const noexcept {
		return {
			unlimited.x ? virtual_dynamic.x : std::min(limits.x, virtual_dynamic.x),
			unlimited.y ? virtual_dynamic.y : std::min(limits.y, virtual_dynamic.y)
		};
	}
};

struct SLC_Test : libv::ui::SLC_Node {
	TestComp& component;

public:
	explicit constexpr LIBV_FORCE_INLINE SLC_Test(TestComp& component) : component(component) {}

public:
	[[nodiscard]] constexpr libv::vec2f calculate_ratio() {
		return ratioSum = component.size.ratio2();
	}

	[[nodiscard]] constexpr libv::vec2f calculate_exclude() {
		return currentSize = {};
	}
	constexpr void assign_primary_size(const SLCSizeConstraints& env) {
		currentSize = component.size.pixel2();

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

		const auto percent = component.size.percent2() * 0.01f * env.parent();
		eval(currentSize.x, component.size.value.x, percent.x);
		eval(currentSize.y, component.size.value.y, percent.y);
	}
	constexpr void assign_dynamic_size(const SLCSizeConstraints& env) {
		if (!component.size.has_dynamic())
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
			component.size.value.x.dynamic ? env.limit().x : currentSize.x,
			component.size.value.y.dynamic ? env.limit().y : currentSize.y
		};
		const auto unlimited = component.size.dynamic2() && env.unlimited();
		const auto dynamic = component.calculate_dynamic(unlimited, limit);
		eval(currentSize.x, component.size.value.x, dynamic.x);
		eval(currentSize.y, component.size.value.y, dynamic.y);
	}
	constexpr void assign_ratio_size(const SLCSizeConstraints& env) {
		currentSize = libv::max(currentSize, env.limit() * component.size.ratio_mask2());
	}

	constexpr void assign_position(libv::vec2f area_position, libv::vec2f area_size, const ContextLayout2& environment) {
		(void) area_size;
		(void) environment;
		// component.layout_position = position + component.margin.LB();
		component.layout_position = area_position;
		component.layout_size = currentSize;
	}
};

// -------------------------------------------------------------------------------------------------
