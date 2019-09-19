// File:   layout_grid.cpp Author: Vader Created on 01 August 2019, 03:58

// hpp
#include <libv/ui/layout/layout_grid.hpp>
// ext
#include <boost/container/small_vector.hpp>
#include <range/v3/view/chunk.hpp>
#include <range/v3/view/enumerate.hpp>
// libv
#include <libv/algorithm/bisect.hpp>
#include <libv/algorithm/sum.hpp>
#include <libv/meta/for_constexpr.hpp>
#include <libv/range/view_deinterleave.hpp>
#include <libv/utility/approx.hpp>
#include <libv/utility/enum.hpp>
#include <libv/utility/observer_ref.hpp>
// pro
#include <libv/ui/base_component.hpp>
#include <libv/ui/context_layout.hpp>
#include <libv/ui/layout/view_layouted.lpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

namespace {

struct Orientation2Data {
	uint32_t _X_;
	uint32_t _Y_;
	uint32_t _Z_;
	libv::vec3f start;
	libv::vec3f direction;
	libv::vec3f penCorner;
};

static constexpr Orientation2Data Orientation2Table[] = {
	{1u, 0u, 2u, {1.f, 1.f, 0.f}, {-1.f, -1.f, +1.f}, {-1.f, -1.f, +0.f}}, // DOWN_LEFT
	{1u, 0u, 2u, {0.f, 1.f, 0.f}, {+1.f, -1.f, +1.f}, {+0.f, -1.f, +0.f}}, // DOWN_RIGHT
	{0u, 1u, 2u, {1.f, 1.f, 0.f}, {-1.f, -1.f, +1.f}, {-1.f, -1.f, +0.f}}, // LEFT_DOWN
	{0u, 1u, 2u, {1.f, 0.f, 0.f}, {-1.f, +1.f, +1.f}, {-1.f, +0.f, +0.f}}, // LEFT_UP
	{0u, 1u, 2u, {0.f, 1.f, 0.f}, {+1.f, -1.f, +1.f}, {+0.f, -1.f, +0.f}}, // RIGHT_DOWN
	{0u, 1u, 2u, {0.f, 0.f, 0.f}, {+1.f, +1.f, +1.f}, {+0.f, +0.f, +0.f}}, // RIGHT_UP
	{1u, 0u, 2u, {1.f, 0.f, 0.f}, {-1.f, +1.f, +1.f}, {-1.f, +0.f, +0.f}}, // UP_LEFT
	{1u, 0u, 2u, {0.f, 0.f, 0.f}, {+1.f, +1.f, +1.f}, {+0.f, +0.f, +0.f}}, // UP_RIGHT
};

inline auto buildLayoutedChildrenRandomAccessRange(libv::span<LayoutGrid::Child> children) {
	boost::container::small_vector<libv::observer_ref<const LayoutGrid::Child>, 32> result;

	for (const auto& child : children | view_layouted())
		result.emplace_back(libv::make_observer_ref(child));

	return result;
}

} // namespace

// -------------------------------------------------------------------------------------------------

libv::vec3f LayoutGrid::layout1(
		const ContextLayout1& environment,
		libv::span<Child> everyChildren,
		const Properties& property,
		const BaseComponent& parent) {

	(void) parent;
	(void) environment;

	const auto children = buildLayoutedChildrenRandomAccessRange(everyChildren);

	const auto column_count = property.column_count();
	const auto& orient = Orientation2Table[libv::to_value(property.orientation2())];
	const auto _X_ = orient._X_;
	const auto _Y_ = orient._Y_;
	const auto _Z_ = orient._Z_;

	auto result = libv::vec3f{};

	for (const auto& child : children) {
		AccessLayout::layout1(*child->ptr, ContextLayout1{});

		result[_Z_] = std::max(result[_Z_],
				child->property.size()[_Z_].pixel +
				(child->property.size()[_Z_].dynamic ? AccessLayout::lastDynamic(*child->ptr)[_Z_] : 0.f));
	}

	const auto attemptX = [&](int32_t parentSize) {
		if (parentSize < 0)
			return -1;

		float parentSizeX = static_cast<float>(parentSize);
		float usedGridSizeX = 0.f;
		for (const auto& column : children | libv::view::deinterleave(column_count)) {
			float usedColumnSizeX = 0.f;
			for (const auto& child : column) {
				usedColumnSizeX = std::max(usedColumnSizeX,
						child->property.size()[_X_].pixel +
						child->property.size()[_X_].percent * parentSizeX * 0.01f +
						(child->property.size()[_X_].dynamic ? AccessLayout::lastDynamic(*child->ptr)[_X_] : 0.f));
			}
			usedGridSizeX += usedColumnSizeX;
		}

		return parentSize - static_cast<int32_t>(usedGridSizeX + 0.5f);
	};

	const auto attemptY = [&](int32_t parentSize) {
		if (parentSize < 0)
			return -1;

		float parentSizeY = static_cast<float>(parentSize);
		float usedGridSizeY = 0.f;
		for (const auto& row : children | ranges::view::chunk(column_count)) {
			float usedRowSizeY = 0.f;
			for (const auto& child : row) {
				usedRowSizeY = std::max(usedRowSizeY,
						child->property.size()[_Y_].pixel +
						child->property.size()[_Y_].percent * parentSizeY * 0.01f +
						(child->property.size()[_Y_].dynamic ? AccessLayout::lastDynamic(*child->ptr)[_Y_] : 0.f));
			}
			usedGridSizeY += usedRowSizeY;
		}

		return parentSize - static_cast<int32_t>(usedGridSizeY + 0.5f);
	};

	// TODO P4: implement a infinite cycle protection (occurs when the percent sum is greaten than 100% with a non zero fix size sum)
	result[_X_] = static_cast<float>(libv::bisect_rampup_3way(attemptX, 0, 0));
	result[_Y_] = static_cast<float>(libv::bisect_rampup_3way(attemptY, 0, 0));

	return result;
}

void LayoutGrid::layout2(
		const ContextLayout2& environment,
		libv::span<Child> everyChildren,
		const Properties& property,
		const BaseComponent& parent) {

	(void) parent;

	const auto children = buildLayoutedChildrenRandomAccessRange(everyChildren);

	// TODO P4: generalized a way for table lookup for various table lookup and handle invalid enum values
	const auto column_count = property.column_count();
	const auto& orient = Orientation2Table[libv::to_value(property.orientation2())];
	const auto _X_ = orient._X_;
	const auto _Y_ = orient._Y_;
	const auto _Z_ = orient._Z_;

	// --- Size ----------------------------------------------------------------------------------------

	// Calculate size ratio contribution
	const auto calculateRatioContribution = [&](const auto _D_, const auto& firstDimensionView) {
		float ratioScale = 0.f;
		bool hasRatio = false;

		// Guarantee initial over-shoot by rewarding the whole parent size to the first found ratio
		for (const auto& child : children) {
			const auto ratio = child->property.size()[_D_].ratio;
			if (ratio != libv::Approx(0.f)) {
				ratioScale = environment.size[_D_] / ratio;
				hasRatio = true;
				break;
			}
		}

		// Not a single ratio found, skip the calculation
		if (not hasRatio)
			return ratioScale;

		// Determine ratio contribution
		while (true) {
			float size = 0.f;
			float ratioCount = 0.f;

			for (const auto& firstDimension : children | firstDimensionView) {
				float firstSubDimSize = 0.f;
				float firstSubDimRatioCount = 0.f;

				for (const auto& child : firstDimension) {
					const auto childSize =
							child->property.size()[_D_].pixel +
							child->property.size()[_D_].percent * environment.size[_D_] * 0.01f +
							child->property.size()[_D_].ratio * ratioScale +
							(child->property.size()[_D_].dynamic ? AccessLayout::lastDynamic(*child->ptr)[_D_] : 0.f);

					if (childSize > firstSubDimSize) {
						firstSubDimSize = childSize;
						firstSubDimRatioCount = child->property.size()[_D_].ratio;
					}
				}

				size += firstSubDimSize;
				ratioCount += firstSubDimRatioCount;
			}

			float overshoot = size - environment.size[_D_];
			ratioScale -= overshoot / ratioCount;

			if (ratioCount == libv::Approx(0.f))
				break;

			if (overshoot == libv::Approx(0.f))
				break;
		}

		return ratioScale;
	};

	const auto calculateRatioContribution3 = [&](const auto _D_) {
		float ratioScale = 0.f;
		bool hasRatio = false;

		// Guarantee initial over-shoot by rewarding the whole parent size to the first found ratio
		for (const auto& child : children) {
			const auto ratio = child->property.size()[_D_].ratio;
			if (ratio != libv::Approx(0.f)) {
				ratioScale = environment.size[_D_] / ratio;
				hasRatio = true;
				break;
			}
		}

		// Not a single ratio found, skip the calculation
		if (not hasRatio)
			return 0.f;

		// Determine ratio contribution
		for (const auto& child : children) {
			if (child->property.size()[_D_].ratio == libv::Approx(0.f))
				continue;

			const auto used =
					child->property.size()[_D_].pixel +
					child->property.size()[_D_].percent * environment.size[_D_] * 0.01f +
					(child->property.size()[_D_].dynamic ? AccessLayout::lastDynamic(*child->ptr)[_D_] : 0.f);
			const auto leftover = environment.size[_D_] - used;
			const auto contribution = leftover / child->property.size()[_D_].ratio;

			ratioScale = std::min(ratioScale, contribution);
		}

		return ratioScale;
	};

	libv::vec3f ratioContribution;
	ratioContribution[_X_] = calculateRatioContribution(_X_, libv::view::deinterleave(column_count));
	ratioContribution[_Y_] = calculateRatioContribution(_Y_, ranges::view::chunk(column_count));
	ratioContribution[_Z_] = calculateRatioContribution3(_Z_);

	// Assign size

	boost::container::small_vector<libv::vec3f, 16> childSizes(children.size(), libv::vec3f{});
	boost::container::small_vector<float, 16> advanceX(column_count, 0.f);
	boost::container::small_vector<float, 16> advanceY(children.size() / column_count + 1, 0.f);

	for (const auto& [y, row] : children | ranges::view::chunk(column_count) | ranges::view::enumerate) {
		for (const auto& [x, child] : row | ranges::view::enumerate) {
			auto& childSize = childSizes[y * column_count + x];

			childSize = libv::build_vec<3>([&](const auto i) {
				return child->property.size()[i].pixel +
						child->property.size()[i].percent * environment.size[i] * 0.01f +
						child->property.size()[i].ratio * ratioContribution[i] +
						(child->property.size()[i].dynamic ? AccessLayout::lastDynamic(*child->ptr)[i] : 0.f);
			});

			advanceX[x] = std::max(advanceX[x], childSize[_X_]);
			advanceY[y] = std::max(advanceY[y], childSize[_Y_]);
		}
	}

	// --- Position ------------------------------------------------------------------------------------

	const auto origin = environment.position;
	auto sizeContent = libv::vec3f{};
	sizeContent[_X_] = libv::sum(advanceX, 0);
	sizeContent[_Y_] = libv::sum(advanceY, 0);

	const auto originToContent = (environment.size - sizeContent) * property.anchor_content();
	const auto contentToStart = sizeContent * orient.start;
	auto startToPen = vec3f{};

	for (const auto& [y, row] : children | ranges::view::chunk(column_count) | ranges::view::enumerate) {
		for (const auto& [x, child] : row | ranges::view::enumerate) {
			const auto& childSize = childSizes[y * column_count + x];

			auto cellSize = libv::vec3f{};
			cellSize[_X_] = advanceX[x];
			cellSize[_Y_] = advanceY[y];
			cellSize[_Z_] = environment.size[_Z_];

			const auto penToCell = orient.penCorner * cellSize;
			const auto cellToPosition = (cellSize - childSize) * child->property.anchor();

			const auto position = origin + originToContent + contentToStart + startToPen + penToCell + cellToPosition;
			const auto roundedPosition = libv::vec::round(position);
			const auto roundedSize = libv::vec::round(position + childSize) - roundedPosition;

			AccessLayout::layout2(
					*child->ptr,
					ContextLayout2{
						roundedPosition,
						roundedSize,
						MouseOrder{libv::to_value(environment.mouseOrder) + 1}
					}
			);

			startToPen[_X_] += orient.direction[_X_] * advanceX[x];
		}
		startToPen[_X_] = 0;
		startToPen[_Y_] += orient.direction[_Y_] * advanceY[y];
	}
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
