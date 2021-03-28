// Project: libv.ui, File: src/libv/ui/component/panel_float.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <libv/ui/component/panel_float.hpp>
// libv
#include <libv/meta/for_constexpr.hpp>
#include <libv/utility/approx.hpp>
#include <libv/utility/min_max.hpp>
#include <libv/utility/to_underlying.hpp>
// pro
#include <libv/ui/component/base_panel.lpp>
#include <libv/ui/context/context_layout.hpp>
#include <libv/ui/context/context_style.hpp>
#include <libv/ui/layout/view_layouted.lpp>
#include <libv/ui/log.hpp>
#include <libv/ui/property_access_context.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class CorePanelFloat : public CoreBasePanel {
	friend class PanelFloat;
	[[nodiscard]] inline auto handler() { return PanelFloat{this}; }

private:
	struct Properties {
		// TODO P5: libv.ui: Implement Snap to edge in float layout
		PropertyL<SnapToEdge> snap_to_edge;
		// TODO P5: libv.ui: Implement Squish in float layout
		PropertyL<Squish> squish;
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

template <typename T>
void CorePanelFloat::access_properties(T& ctx) {
	ctx.property(
			[](auto& c) -> auto& { return c.snap_to_edge; },
			SnapToEdge{false},
			pgr::layout, pnm::snap_to_edge,
			"Snap to edge any child that otherwise would hang out"
	);
	ctx.property(
			[](auto& c) -> auto& { return c.squish; },
			Squish{false},
			pgr::layout, pnm::squish,
			"Squish any child that otherwise would hang out"
	);
}

template <typename T>
void CorePanelFloat::access_child_properties(T& ctx) {
	(void) ctx;
}

// -------------------------------------------------------------------------------------------------

void CorePanelFloat::doStyle(ContextStyle& ctx) {
	PropertyAccessContext<Properties> setter{property, ctx.component, ctx.style, ctx.component.context()};
	access_properties(setter);
	CoreComponent::access_properties(setter);
}

void CorePanelFloat::doStyle(ContextStyle& ctx, ChildID childID) {
	(void) ctx;
	(void) childID;
}

// -------------------------------------------------------------------------------------------------

libv::vec3f CorePanelFloat::doLayout1(const ContextLayout1& layout_env) {
	const auto env_size = layout_env.size - padding_size3();

	const auto resolvePercent = [](const float fix, const float percent, auto& component) {
		if (fix == libv::Approx(0.f)) {
			return fix;
		} else if (percent == libv::Approx(100.f)) {
			log_ui.warn("Invalid sum of size percent {} with fixed width of {} during layout of {}", percent, fix, component.path());
			return fix * 2.f;
		} else {
			return fix / (1.f - percent * 0.01f);
		}
	};

	auto result = libv::vec3f{};

	for (auto& child : children | view_layouted()) {
		const auto child_dynamic = child.size().has_dynamic() ?
				AccessLayout::layout1(child.core(), ContextLayout1{env_size}) :
				libv::vec3f{};

		libv::meta::for_constexpr<0, 3>([&](auto i) {
			result[i] = libv::max(
					result[i],
					resolvePercent(
							child.size()[i].pixel + (child.size()[i].dynamic ? child_dynamic[i] : 0.f),
							child.size()[i].percent, child.core())
			);
		});
	}

	return result + padding_size3();
}

void CorePanelFloat::doLayout2(const ContextLayout2& layout_env) {
	const auto env_size = layout_env.size - padding_size3();

	for (auto& child : children | view_layouted()) {

		// Size ---

		auto size = libv::vec3f{};

		const auto child_dynamic = child.size().has_dynamic() ?
				AccessLayout::layout1(child.core(), ContextLayout1{env_size}) :
				libv::vec3f{};

		libv::meta::for_constexpr<0, 3>([&](auto i) {
			const auto has_ratio = child.size()[i].ratio != 0.f;

			if (has_ratio)
				size[i] = env_size[i];
			else
				size[i] =
						child.size()[i].pixel +
						child.size()[i].percent * 0.01f * env_size[i] +
						(child.size()[i].dynamic ? child_dynamic[i] : 0.f);
		});

		// Position ---

		const auto position =
				+ padding_LB3()
				+ child.anchor().to_info() * env_size
				- child.anchor().to_info() * size;

		const auto roundedPosition = libv::vec::round(position);
		const auto roundedSize = libv::vec::round(position + size) - roundedPosition;

		AccessLayout::layout2(
				child.core(),
				layout_env.enter(roundedPosition, roundedSize)
		);
	}
}

// =================================================================================================

PanelFloat::PanelFloat(std::string name) :
	ComponentHandler<CorePanelFloat, EventHostGeneral<PanelFloat>>(std::move(name)) { }

PanelFloat::PanelFloat(GenerateName_t gen, const std::string_view type) :
	ComponentHandler<CorePanelFloat, EventHostGeneral<PanelFloat>>(gen, type) { }

PanelFloat::PanelFloat(core_ptr core) noexcept :
	ComponentHandler<CorePanelFloat, EventHostGeneral<PanelFloat>>(core) { }

// -------------------------------------------------------------------------------------------------

void PanelFloat::snap_to_edge(SnapToEdge value) {
	AccessProperty::manual(self(), self().property.snap_to_edge, value);
}

SnapToEdge PanelFloat::snap_to_edge() const noexcept {
	return self().property.snap_to_edge();
}

void PanelFloat::squish(Squish value) {
	AccessProperty::manual(self(), self().property.squish, value);
}

Squish PanelFloat::squish() const noexcept {
	return self().property.squish();
}

// -------------------------------------------------------------------------------------------------

void PanelFloat::add(Component component) {
	self().add(std::move(component));
}

void PanelFloat::remove(Component& component) {
	self().remove(component);
}

void PanelFloat::clear() {
	self().clear();
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
