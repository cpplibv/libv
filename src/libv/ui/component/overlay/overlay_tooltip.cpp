// Created by dalma.bobula on 12/3/2022.

// hpp
#include <libv/ui/component/overlay/overlay_tooltip.hpp>
// pro
//#include <libv/ui/property/margin.hpp>
#include <libv/ui/component/component.hpp>
#include <libv/ui/component/base_panel_core.hpp>
#include <libv/ui/component/layout/layout_slc.hpp>
#include <libv/algo/erase_unstable.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------


class Tooltip {
// private:
	// struct TooltipProperties {
	// 	AnchorType anchorType;
	// 	Margin anchorClearance; //left/right/bottom/top pixel count
	// 	Alignment alignment = Alignment::bottom_right;
	// 	Edge edge;
	// 	EdgeBehaviour edgeBehaviour;
	// 	Clipping clipping;
	// };

public:
	// TooltipProperties properties;
	// PanelAnchor container;
	Component target;
	Component component;

	Anchor target_anchor;
	Anchor source_anchor;

	libv::vec2f target_prev_pos;
	libv::vec2f target_prev_size;
};

struct CoreOverlayTooltip : public CoreBasePanel {
	using base_type = CoreBasePanel;
	using base_type::base_type;

public:

	std::vector<Tooltip> tooltips;


	template <typename Access>
	void access_layout(Access&& access) {
		for (auto& tooltip: tooltips) {
			access(
					SLC_Anchor(tooltip.source_anchor, tooltip.target_anchor, tooltip.target,
						SLC_Padding(this->padding_extent(), SLC_Component(tooltip.component))
					)
			);
		}
	}

public:
	void doUpdate() override {
		for (auto& tooltip: tooltips) {
			//If tooltip.target pos chanchaged relayout tooltip
			if (tooltip.target.layout_position2() != tooltip.target_prev_pos || tooltip.target.layout_size2() != tooltip.target_prev_size) {
				//No need to precache bounds as during 'add' invalidation will always happen
				tooltip.target_prev_pos = tooltip.target.layout_position2();
				tooltip.target_prev_size = tooltip.target.layout_size2();

				markInvalidLayout(false, false);
			}
		}
	}

	libv::vec3f doLayout1(const ContextLayout1& layout_env) override {
		libv::vec2f result;
		access_layout([&](auto&& plan) {
			result = libv::ui::layoutSLCPass1(layout_env.size, plan);
		});
		return {result, 0};
	}

	void doLayout2(const ContextLayout2& environment) override {
		access_layout([&](auto&& plan) {
			libv::ui::layoutSLCPass2(environment.size, environment, plan);
		});
	}

	void show_tooltip(Component target, Component tooltip,
			 Anchor target_anchor = Anchor::bottom_right, Anchor source_anchor = Anchor::top_left) {
		add(tooltip);

		// !!! TODO P1: Capturing this could result in segfault
		// TODO P4: What about card layout?
		target.event().detach.connect_system([tooltip, this]() mutable {
			remove(tooltip);
			libv::erase_unstable(tooltips, tooltip, &Tooltip::component);
		});
		tooltips.emplace_back(std::move(target), std::move(tooltip), target_anchor, source_anchor);
	}

};

// -------------------------------------------------------------------------------------------------

core_ptr OverlayTooltip::create_core(std::string name) {
	return create_core_ptr<CoreType>(std::move(name));
}

bool OverlayTooltip::castable(libv::ui::core_ptr core) noexcept {
	return dynamic_cast<CoreType*>(core) != nullptr;
}

void OverlayTooltip::show_tooltip(Component target, Component tooltip) {
	self().show_tooltip(std::move(target), std::move(tooltip));
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
