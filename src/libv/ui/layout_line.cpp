// File: line_layout.cpp - Created on 2017.10.18. 03:41 - Author: Vader

// hpp
#include <libv/ui/layout_line.hpp>
// libv
#include <libv/math/vec.hpp>
#include <libv/utility/min_max.hpp>
// pro
#include <libv/ui/component_base.hpp>
#include <libv/ui/context_layout.hpp>
#include <libv/ui/log.hpp>
#include <libv/ui/size.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

void LayoutLine::layoutPass1(const ContextLayoutPass1& environment, ComponentBase& component) {
	(void) environment;

	const auto& orientData = orient.get();
	const auto dim1 = orientData.dim1;
	const auto dim2 = orientData.dim2;
	const auto dim3 = orientData.dim3;

	auto contentSize = libv::vec3f{};
	auto dim1Percent = 0.f;
	auto dim1Pixel = 0.f;
	auto dim1Content = 0.f;

	const auto resolvePercent = [&component](auto&& fix, auto&& percent) {
		if (fix < 0.01f) {
			return fix;
		} else if (percent > 99.99f) {
			// TODO P3: Print layout stack
			log_ui.error("Invalid sum of size percent {} with fixed width of {} during layout of {}", percent, fix, component.path());
			return fix * 2.f;
		} else {
			return fix / (1.f - percent * 0.01f);
		}
	};

	component.foreachChildren([&](ComponentBase & child) {
		child.layoutPass1(ContextLayoutPass1{});

		dim1Pixel += child.propertySize[dim1].pixel;
		dim1Percent += child.propertySize[dim1].percent;
		dim1Content += child.lastContent[dim1];

		contentSize[dim2] = libv::max(
				contentSize[dim2],
				resolvePercent(child.propertySize[dim2].pixel, child.propertySize[dim2].percent),
				child.lastContent[dim2]
		);

		contentSize[dim3] = libv::max(
				contentSize[dim3],
				resolvePercent(child.propertySize[dim3].pixel, child.propertySize[dim3].percent),
				child.lastContent[dim3]
		);
	});

	contentSize[dim1] = libv::max(
			resolvePercent(dim1Pixel, dim1Percent),
			dim1Content
	);

	component.lastContent = contentSize;
}

void LayoutLine::layoutPass2(const ContextLayoutPass2& environment, ComponentBase& component) {
	const auto& alignHData = alignHorizontal.get();
	const auto& alignVData = alignVertical.get();
	const auto& orientData = orient.get();

	const auto dim1 = orientData.dim1;
	const auto dim2 = orientData.dim2;
	const auto dim3 = orientData.dim3;

	// Size ---

	auto dim1Content = 0.f;
	auto dim1SumRatio = 0.f;
	auto numComponent = size_t{0};

	component.foreachChildren([&](ComponentBase& child) {
		libv::meta::n_times_index<3>([&](auto&& index) {
			child.size[index] =
					child.propertySize[index].pixel +
					child.propertySize[index].percent * 0.01f * environment.size[index] +
					(child.propertySize[index].content ? child.lastContent[index] : 0.f);
		});

		dim1SumRatio += child.propertySize[dim1].ratio;
		dim1Content += child.size[dim1];
		++numComponent;
	});

	auto dim1Leftover = environment.size[dim1] - dim1Content;
	auto dim1RatioScaled = dim1SumRatio > 0.f ? dim1Leftover / dim1SumRatio : 0.f;
	auto sizeContent = vec3f{};

	component.foreachChildren([&](ComponentBase& child) {
		child.size[dim1] += dim1RatioScaled * child.propertySize[dim1].ratio;
		if (child.propertySize[dim2].ratio > 0.f) child.size[dim2] = environment.size[dim2];
		if (child.propertySize[dim3].ratio > 0.f) child.size[dim3] = environment.size[dim3];

		sizeContent[dim1] += child.size[dim1];
		sizeContent[dim2] = std::max(sizeContent[dim2], child.size[dim2]);
		sizeContent[dim3] = std::max(sizeContent[dim3], child.size[dim3]);
	});

	// Position ---

	auto environmentUnused = environment.size - sizeContent;
	const auto dim1JustifyGap = alignHData.justifyGap * environmentUnused[dim1] / std::max(static_cast<float>(numComponent) - 1.0f, 1.0f);
	if (alignHData.justified)
		environmentUnused[dim1] = 0.0f;

	const auto aligmentScale = vec3f{alignHData.scale, alignVData.scale, 0.0f};

	const auto origin = environment.position;
	const auto originToContent = environmentUnused * aligmentScale;
	const auto contentToStart = sizeContent * orientData.start;

	auto startToPen = vec3f{};

	component.foreachChildren([&](ComponentBase& child) {
		const auto penToBase = child.size * orientData.penCorner;
		auto baseToPosition = vec3f{};
		baseToPosition[dim2] = (sizeContent[dim2] - child.size[dim2]) * aligmentScale[dim2];
		const auto position = origin + originToContent + contentToStart + startToPen + penToBase + baseToPosition;

		startToPen[dim1] += orientData.direction[dim1] * child.size[dim1] + dim1JustifyGap;

		const auto roundedPosition = libv::vec::round(position);
		const auto roundedSize = libv::vec::round(position + child.size) - roundedPosition;
		child.layoutPass2(ContextLayoutPass2{roundedPosition, roundedSize});
	});
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
