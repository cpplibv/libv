// Created by Vader on 2021.12.03..

#pragma once

// ext
#include <boost/container/small_vector.hpp>
// libv
#include <libv/math/vec.hpp>
#include <libv/utility/approx.hpp>
// pro
#include <libv/ui/component/layout/view_layouted.hxx>
#include <libv/ui/context/context_layout.hpp>
#include <libv/ui/log.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

constexpr inline auto resolvePercent(const float fix, const float percent, auto& component) {
	if (fix == libv::approx(0.f)) {
		return fix;
	} else if (percent == libv::approx(100.f)) {
		log_ui.warn("Invalid sum of size percent {} with fixed width of {} during layout of {}", percent, fix, component.path());
		return fix * 2.f;
	} else {
		return fix / (1.f - percent * 0.01f);
	}
}

constexpr inline libv::vec3f build_vec3f(uint32_t dim0, float value0) {
	libv::vec3f result;
	result[dim0] = value0;
	return result;
}

constexpr inline libv::vec3f build_vec3f(uint32_t dim0, float value0, uint32_t dim1, float value1) {
	libv::vec3f result;
	result[dim0] = value0;
	result[dim1] = value1;
	return result;
}

constexpr inline libv::vec3f build_vec3f(uint32_t dim0, float value0, uint32_t dim1, float value1, uint32_t dim2, float value2) {
	libv::vec3f result;
	result[dim0] = value0;
	result[dim1] = value1;
	result[dim2] = value2;
	return result;
}

constexpr inline libv::vec2f swizzle(libv::vec2f value0, uint32_t dim0, uint32_t dim1) {
	return libv::vec2f{value0[dim0], value0[dim1]};
}

constexpr inline libv::vec2f swizzle(libv::vec3f value0, uint32_t dim0, uint32_t dim1) {
	return libv::vec2f{value0[dim0], value0[dim1]};
}

constexpr inline libv::vec2f unswizzle(libv::vec2f value0, uint32_t dim0, uint32_t dim1) {
	libv::vec2f result;
	result[dim0] = value0.x;
	result[dim1] = value0.y;
	return result;
}

constexpr inline libv::vec3f calculate_layout1_contrib(auto& core, libv::vec3f max_size) {
	auto result = libv::vec3f{};

	if (!core.size().has_dynamic())
		return result;

	const auto dynamic_size = AccessLayout::layout1(core, ContextLayout1{max_size});

	if (core.size()[0].dynamic)
		result.x = dynamic_size.x;
	if (core.size()[1].dynamic)
		result.y = dynamic_size.y;
	if (core.size()[2].dynamic)
		result.z = dynamic_size.z;

	return result;
};

constexpr inline libv::vec2f calculate_layout1_contrib(auto& core, libv::vec2f max_size) {
	return xy(calculate_layout1_contrib(core, libv::vec3f{max_size, 0.f}));
};

// -------------------------------------------------------------------------------------------------

struct LayoutEntry {
	CoreComponent* child;

	libv::vec2_t<SizeDim> propertySizeSW; /// Swizzled
	libv::vec2f paddingSizeSW; /// Swizzled
	libv::vec2f marginSizeSW; /// Swizzled

	float marginFrontX; /// Swizzled
	float marginBackX; /// Swizzled
	float marginBottomY; /// Swizzled

	libv::vec2f resultSizeSW; /// Swizzled
};

inline auto build_entries(
		boost::container::small_vector<LayoutEntry, 32>& entries,
		std::vector<Component>& children,
		uint32_t swizzleX,
		uint32_t swizzleY,
		uint32_t swizzleFront,
		uint32_t swizzleBack,
		uint32_t swizzleBottom) {

	entries.resize(children.size()); // Note: This resize may allocate more memory as children may include non-layouted components too, but that is rare and saves a pass

	int entries_num = 0;
	for (auto& c : children | view_layouted()) {
		auto& entry = entries[entries_num++];
		entry.child = &c.core();

		const auto& propertySize = c.size();
		entry.propertySizeSW.x = propertySize[swizzleX];
		entry.propertySizeSW.y = propertySize[swizzleY];

		entry.paddingSizeSW = swizzle(c.padding_size(), swizzleX, swizzleY);
		entry.marginSizeSW = swizzle(c.margin_size(), swizzleX, swizzleY);

		entry.marginFrontX = c.margin()[swizzleFront];
		entry.marginBackX = c.margin()[swizzleBack];
		entry.marginBottomY = c.margin()[swizzleBottom];
	}

	entries.resize(entries_num);
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
