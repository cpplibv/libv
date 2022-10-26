// Project: libv.ui, File: src/libv/ui/component/scroll_pane.hpp

#pragma once

// pro
#include <libv/ui/component_system/component_api.hpp>
#include <libv/ui/component/fwd.hpp>
#include <libv/ui/component/scroll_area.hpp>
#include <libv/ui/property/background.hpp>
#include <libv/ui/property/scroll_mode.hpp>
#include <libv/ui/property/spacing.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

//enum class BarVisibility {
//	/// The default. The scroll bar is displayed when the client area does not fit in the viewport.
//	on_demand = 0,
//
//	/// The scroll bar appears when the viewport is smaller than the client and disappears when the viewport is larger than the client.
//	on_demand_lazy,
//
//	/// Always display the scroll bar. The knob disappears if the viewport is large enough to show the whole client.
//	always,
//
//	/// Never display the scroll bar. Use this option if you don't want the user to directly control what part of the client is shown, or if you want them to use only non-scroll-bar techniques (such as dragging).
//	never,
//};
//
//enum class ScrollPaneMode {
//	horizontal,
//	vertical,
//	both_with_horizontal_main,
//
//	none,
//	both_with_corner,
//	both_with_vertical_main,
//};
//
//enum class BarPlacementHorizontal {
//	none,
//	left,
//	left_inner,
//	left_inner_consider_occluding, // occluding means it will allow over-scrolling to uncover area under the scroll-bar
//	right,
//	right_inner,
//	right_inner_consider_occluding,
//	both,
//	both_inner,
//	both_inner_consider_occluding,
//};
//
//enum class BarPlacementVertical {
//	none,
//	top,
//	top_inner,
//	top_inner_consider_occluding,
//	bottom,
//	bottom_inner,
//	bottom_inner_consider_occluding,
//	both,
//	both_inner,
//	both_inner_consider_occluding,
//};

// -------------------------------------------------------------------------------------------------

class ScrollPane : public ComponentAPI<Component, ScrollPane, class CoreScrollPane, EventHostScrollArea> {
public:
	using ComponentAPI::ComponentAPI;
	static constexpr std::string_view component_type = "s-pane";
	[[nodiscard]] static core_ptr create_core(std::string name);
	[[nodiscard]] static bool castable(core_ptr) noexcept;

public:
	void background(Background value);
	[[nodiscard]] const Background& background() const noexcept;

public:
	void mode(ScrollMode value) noexcept;
	[[nodiscard]] ScrollMode mode() const noexcept;

	void content(Component&& value) noexcept;
	void content(const Component& value) noexcept;
	[[nodiscard]] Component& content() noexcept;
	[[nodiscard]] const Component& content() const noexcept;

//public:
//	void seek_to(const Component& component) noexcept;

public:
	void area_position(libv::vec2f value) noexcept;
	[[nodiscard]] libv::vec2f area_position() const noexcept;

	[[nodiscard]] libv::vec2f area_size() const noexcept;

	void spacing2(Spacing2 value);
	[[nodiscard]] Spacing2 spacing2() const noexcept;

//	[[nodiscard]] ScrollArea& scroll_area();
//	[[nodiscard]] const ScrollArea& scroll_area() const;

	[[nodiscard]] Slider& bar_vertical() noexcept;
	[[nodiscard]] const Slider& bar_vertical() const noexcept;

	[[nodiscard]] Slider& bar_horizontal() noexcept;
	[[nodiscard]] const Slider& bar_horizontal() const noexcept;

//	void corner(Component value);
//	void clear_corner() noexcept;
//	[[nodiscard]] bool has_corner() const noexcept;
//	[[nodiscard]] const Component& corner() const;
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
