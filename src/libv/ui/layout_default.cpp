// File: line_layout.cpp - Created on 2017.10.18. 03:41 - Author: Vader

// hpp
#include <libv/ui/layout_default.hpp>
// libv
#include <libv/math/vec.hpp>
#include <libv/meta/n_times.hpp>
#include <libv/utility/min_max.hpp>
// pro
#include <libv/ui/component_base.hpp>
#include <libv/ui/context_layout.hpp>
#include <libv/ui/log.hpp>
#include <libv/ui/property/size.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

void LayoutDefault::layoutPass1(const ContextLayoutPass1& environment, ComponentBase& component) {
	(void) environment;

	auto contentSize = libv::vec3f{};

	// TODO P5: move resolvePercent to some common layout header
	const auto resolvePercent = [&component](const float fix, const float percent) {
		if (fix < 0.01f) {
			return fix;
		} else if (percent > 99.99f) {
			log_ui.error("Invalid sum of size percent {} with fixed width of {} during layout of {}", percent, fix, component.path());
			return fix * 2.f;
		} else {
			return fix / (1.f - percent * 0.01f);
		}
	};

	component.foreachChildren([&](ComponentBase & child) {
		child.layoutPass1(ContextLayoutPass1{});

		libv::meta::for_constexpr<0, 3>([&](auto i) {
			contentSize[i] = libv::max(
					contentSize[i],
					resolvePercent(child.propertySize[i].pixel, child.propertySize[i].percent),
					child.lastContent[i]
			);
		});
	});

	component.lastContent = contentSize;
}

void LayoutDefault::layoutPass2(const ContextLayoutPass2& environment, ComponentBase& component) {
	component.foreachChildren([&](ComponentBase& child) {
		child.position = environment.position;
		child.size = environment.size;

		child.layoutPass2(environment);
	});
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
