// File: label.cpp - Created on 2017.11.05. 07:52 - Author: Vader

// hpp
#include <libv/ui/component/panel_grid.hpp>
// ext
#include <boost/container/small_vector.hpp>
#include <range/v3/view/chunk.hpp>
#include <range/v3/view/enumerate.hpp>
//#include <range/v3/view/zip.hpp>
// libv
#include <libv/algorithm/bisect.hpp>
#include <libv/algorithm/sum.hpp>
#include <libv/range/view_deinterleave.hpp>
#include <libv/utility/approx.hpp>
#include <libv/utility/enum.hpp>
#include <libv/utility/observer_ref.hpp>
// pro
#include <libv/ui/core_component.hpp>
#include <libv/ui/component/base_panel.lpp>
#include <libv/ui/context/context_layout.hpp>
#include <libv/ui/context/context_style.hpp>
#include <libv/ui/layout/view_layouted.lpp>
#include <libv/ui/property_access_context.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class CorePanelGrid : public CoreBasePanel {
	friend class PanelGrid;
	[[nodiscard]] inline auto handler() { return PanelGrid{this}; }

private:
	struct Properties {
		PropertyL<ColumnCount> column_count;
		PropertyL<Orientation2> orientation2;
	} property;

	struct ChildProperties {
	};

	template <typename T> static void access_properties(T& ctx);
	template <typename T> static void access_child_properties(T& ctx);

//	static ComponentPropertyDescription description;
//	static ComponentPropertyDescription child_description;

public:
	using CoreBasePanel::CoreBasePanel;

private:
	virtual void doStyle(ContextStyle& context) override;
	virtual void doStyle(ContextStyle& context, ChildID childID) override;
	virtual libv::vec3f doLayout1(const ContextLayout1& le) override;
	virtual void doLayout2(const ContextLayout2& le) override;
};

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

template <typename Range>
inline auto buildLayoutedChildrenRandomAccessRange(Range& children, libv::vec3f env_bounds) {
	// Build a random access range from the layouted children

	struct Element {
		libv::observer_ref<Component> component;
		libv::vec3f dynamic;

		inline Component* operator->() noexcept {
			return &*component;
		}
		inline const Component* operator->() const noexcept {
			return &*component;
		}
	};

	boost::container::small_vector<Element, 32> result;

	for (auto& child : children | view_layouted()) {
		const auto child_dynamic = child.size().has_dynamic() ?
				AccessLayout::layout1(child.core(), ContextLayout1{env_bounds}) :
				libv::vec3f{};

		result.emplace_back(libv::make_observer_ref(child), child_dynamic);
	}

	return result;
}

} // namespace

// -------------------------------------------------------------------------------------------------

template <typename T>
void CorePanelGrid::access_properties(T& ctx) {
	ctx.property(
			[](auto& c) -> auto& { return c.column_count; },
			ColumnCount{2},
			pgr::layout, pnm::column_count,
			"Column count of the secondary dimension"
	);
	ctx.property(
			[](auto& c) -> auto& { return c.orientation2; },
			Orientation2::RIGHT_DOWN,
			pgr::layout, pnm::orientation2,
			"Two dimensional orientation of subsequent components"
	);
}

template <typename T>
void CorePanelGrid::access_child_properties(T& ctx) {
	(void) ctx;
}

// -------------------------------------------------------------------------------------------------

void CorePanelGrid::doStyle(ContextStyle& ctx) {
	PropertyAccessContext<Properties> setter{property, ctx.component, ctx.style, ctx.component.context()};
	access_properties(setter);
	CoreComponent::access_properties(setter);
}

void CorePanelGrid::doStyle(ContextStyle& ctx, ChildID childID) {
	(void) ctx;
	(void) childID;
}

// -------------------------------------------------------------------------------------------------

libv::vec3f CorePanelGrid::doLayout1(const ContextLayout1& layout_env) {
	const auto env_size = layout_env.size - padding_size3();

	const auto l_children = buildLayoutedChildrenRandomAccessRange(children, env_size);

	const auto column_count = property.column_count();
	const auto& orient = Orientation2Table[libv::to_value(property.orientation2())];
	const auto _X_ = orient._X_;
	const auto _Y_ = orient._Y_;
	const auto _Z_ = orient._Z_;

	auto result = libv::vec3f{};

	boost::container::small_vector<libv::vec3f, 32> child_dynamics(children.size(), libv::vec3f{});

	for (auto& child : l_children) {
		result[_Z_] = std::max(result[_Z_],
				child->size()[_Z_].pixel +
				(child->size()[_Z_].dynamic ? child.dynamic[_Z_] : 0.f));
	}

	const auto attemptX = [&](int32_t parentSize) {
		if (parentSize < 0)
			return -1;

		float parentSizeX = static_cast<float>(parentSize);
		float usedGridSizeX = 0.f;
		for (const auto& column : l_children | libv::view::deinterleave(column_count)) {
			float usedColumnSizeX = 0.f;
			for (const auto& child : column) {
				usedColumnSizeX = std::max(usedColumnSizeX,
						child->size()[_X_].pixel +
						child->size()[_X_].percent * parentSizeX * 0.01f +
						(child->size()[_X_].dynamic ? child.dynamic[_X_] : 0.f));
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
		for (const auto& row : l_children | ranges::view::chunk(column_count)) {
			float usedRowSizeY = 0.f;
			for (const auto& child : row) {
				usedRowSizeY = std::max(usedRowSizeY,
						child->size()[_Y_].pixel +
						child->size()[_Y_].percent * parentSizeY * 0.01f +
						(child->size()[_Y_].dynamic ? child.dynamic[_Y_] : 0.f));
			}
			usedGridSizeY += usedRowSizeY;
		}

		return parentSize - static_cast<int32_t>(usedGridSizeY + 0.5f);
	};

	// TODO P4: implement a infinite cycle protection (occurs when the percent sum is greaten than 100% with a non zero fix size sum)
	result[_X_] = static_cast<float>(libv::bisect_rampup_3way(attemptX, 0, 0));
	result[_Y_] = static_cast<float>(libv::bisect_rampup_3way(attemptY, 0, 0));

	return result + padding_size3();
}

void CorePanelGrid::doLayout2(const ContextLayout2& layout_env) {
	const auto env_size = layout_env.size - padding_size3();

	const auto l_children = buildLayoutedChildrenRandomAccessRange(children, env_size);

	// TODO P4: generalize a way for table lookup for various table lookup and handle invalid enum values
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
		for (const auto& child : l_children) {
			const auto ratio = child->size()[_D_].ratio;
			if (ratio != libv::Approx(0.f)) {
				ratioScale = env_size[_D_] / ratio;
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

			for (const auto& firstDimension : l_children | firstDimensionView) {
				float firstSubDimSize = 0.f;
				float firstSubDimRatioCount = 0.f;

				for (const auto& child : firstDimension) {
					const auto childSize =
							child->size()[_D_].pixel +
							child->size()[_D_].percent * env_size[_D_] * 0.01f +
							child->size()[_D_].ratio * ratioScale +
							(child->size()[_D_].dynamic ? child.dynamic[_D_] : 0.f);

					if (childSize > firstSubDimSize) {
						firstSubDimSize = childSize;
						firstSubDimRatioCount = child->size()[_D_].ratio;
					}
				}

				size += firstSubDimSize;
				ratioCount += firstSubDimRatioCount;
			}

			float overshoot = size - env_size[_D_];
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
		for (const auto& child : l_children) {
			const auto ratio = child->size()[_D_].ratio;
			if (ratio != libv::Approx(0.f)) {
				ratioScale = env_size[_D_] / ratio;
				hasRatio = true;
				break;
			}
		}

		// Not a single ratio found, skip the calculation
		if (not hasRatio)
			return 0.f;

		// Determine ratio contribution
		for (auto& child : l_children) {
			if (child->size()[_D_].ratio == libv::Approx(0.f))
				continue;

			const auto used =
					child->size()[_D_].pixel +
					child->size()[_D_].percent * env_size[_D_] * 0.01f +
					(child->size()[_D_].dynamic ? child.dynamic[_D_] : 0.f);
			const auto leftover = env_size[_D_] - used;
			const auto contribution = leftover / child->size()[_D_].ratio;

			ratioScale = std::min(ratioScale, contribution);
		}

		return ratioScale;
	};

	libv::vec3f ratioContribution;
	ratioContribution[_X_] = calculateRatioContribution(_X_, libv::view::deinterleave(column_count));
	ratioContribution[_Y_] = calculateRatioContribution(_Y_, ranges::view::chunk(column_count));
	ratioContribution[_Z_] = calculateRatioContribution3(_Z_);

	// Assign size

	boost::container::small_vector<libv::vec3f, 32> childSizes(l_children.size(), libv::vec3f{});
	boost::container::small_vector<float, 32> advanceX(column_count, 0.f);
	boost::container::small_vector<float, 32> advanceY(l_children.size() / column_count + 1, 0.f);

	for (const auto& [y, row] : l_children | ranges::view::chunk(column_count) | ranges::view::enumerate) {
		for (const auto& [x, child] : row | ranges::view::enumerate) {
			auto& childSize = childSizes[y * column_count + x];

			childSize = libv::build_vec<3>([&](const auto i) {
				return child->size()[i].pixel +
						child->size()[i].percent * env_size[i] * 0.01f +
						child->size()[i].ratio * ratioContribution[i] +
						(child->size()[i].dynamic ? child.dynamic[i] : 0.f);
			});

			advanceX[x] = std::max(advanceX[x], childSize[_X_]);
			advanceY[y] = std::max(advanceY[y], childSize[_Y_]);
		}
	}

	// --- Position ------------------------------------------------------------------------------------

	auto sizeContent = libv::vec3f{};
	sizeContent[_X_] = libv::sum(advanceX, 0);
	sizeContent[_Y_] = libv::sum(advanceY, 0);

	const auto originToContent = (env_size - sizeContent) * anchor().to_info();
	const auto contentToStart = sizeContent * orient.start;
	auto startToPen = vec3f{};

	for (const auto& [y, row] : l_children | ranges::view::chunk(column_count) | ranges::view::enumerate) {
		for (const auto& [x, child] : row | ranges::view::enumerate) {
			const auto& childSize = childSizes[y * column_count + x];

			auto cellSize = libv::vec3f{};
			cellSize[_X_] = advanceX[x];
			cellSize[_Y_] = advanceY[y];
			cellSize[_Z_] = env_size[_Z_];

			const auto penToCell = orient.penCorner * cellSize;
			const auto cellToPosition = (cellSize - childSize) * child->anchor().to_info();

			const auto position = padding_LB3() + originToContent + contentToStart + startToPen + penToCell + cellToPosition;
			const auto roundedPosition = libv::vec::round(position);
			const auto roundedSize = libv::vec::round(position + childSize) - roundedPosition;

			AccessLayout::layout2(
					child.component->core(),
					layout_env.enter(roundedPosition, roundedSize)
			);

			startToPen[_X_] += orient.direction[_X_] * advanceX[x];
		}
		startToPen[_X_] = 0;
		startToPen[_Y_] += orient.direction[_Y_] * advanceY[y];
	}
}

// =================================================================================================

PanelGrid::PanelGrid(std::string name) :
	ComponentHandler<CorePanelGrid, EventHostGeneral<PanelGrid>>(std::move(name)) { }

PanelGrid::PanelGrid(GenerateName_t gen, const std::string_view type) :
	ComponentHandler<CorePanelGrid, EventHostGeneral<PanelGrid>>(gen, type) { }

PanelGrid::PanelGrid(core_ptr core) noexcept :
	ComponentHandler<CorePanelGrid, EventHostGeneral<PanelGrid>>(core) { }

// -------------------------------------------------------------------------------------------------

void PanelGrid::column_count(ColumnCount value) {
	AccessProperty::manual(self(), self().property.column_count, value);
}

ColumnCount PanelGrid::column_count() const noexcept{
	return self().property.column_count();
}

void PanelGrid::orientation2(Orientation2 value) {
	AccessProperty::manual(self(), self().property.orientation2, value);
}

Orientation2 PanelGrid::orientation2() const noexcept{
	return self().property.orientation2();
}

// -------------------------------------------------------------------------------------------------

void PanelGrid::add(Component component) {
	self().add(std::move(component));
}

void PanelGrid::remove(Component& component) {
	self().remove(component);
}

void PanelGrid::clear() {
	self().clear();
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
