// File: label.hpp - Created on 2017.10.22. 07:12 - Author: Vader

#pragma once

// pro
#include <libv/ui/component.hpp>
#include <libv/ui/property/align.hpp>
#include <libv/ui/property/color.hpp>
#include <libv/ui/property/orientation.hpp>
#include <libv/ui/property/shader_image.hpp>
#include <libv/ui/property/texture_2D.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

struct EventScrollChange : BaseEvent {
	double change;

	constexpr inline EventScrollChange(double change) noexcept : change(change) { }
};

template <typename ComponentT>
struct EventHostScroll : EventHostGeneral<ComponentT> {
	BasicEventProxy<ComponentT, EventScrollChange> change;

	EventHostScroll(ComponentT& core) : EventHostGeneral<ComponentT>(core),
		change(core) {}
};

// -------------------------------------------------------------------------------------------------

// Notes for scroll pane:
//
// HORIZONTAL_SCROLLBAR_ALWAYS Always display the scroll bar. The knob disappears if the viewport is large enough to show the whole client.
// HORIZONTAL_SCROLLBAR_AS_NEEDED The default. The scroll bar appears when the viewport is smaller than the client and disappears when the viewport is larger than the client.
// HORIZONTAL_SCROLLBAR_NEVER Never display the scroll bar. Use this option if you don't want the user to directly control what part of the client is shown, or if you want them to use only non-scroll-bar techniques (such as dragging).
// HORIZONTAL_SCROLLBAR_aggressive_hide
// VERTICAL_SCROLLBAR_ALWAYS
// VERTICAL_SCROLLBAR_AS_NEEDED
// VERTICAL_SCROLLBAR_NEVER
// VERTICAL_SCROLLBAR_aggressive_hide
//
//		double value; // current_line
//		double value_min; // 0
//		double value_max; // num_line
//		double value_range; // num_lines_displayed
//
//		double step_scroll; // [mouse wheel] 1 or 3 or settings.scroll_unit
//		double step_button; // [button >] 1 or 3 or settings.scroll_unit
//		double step_track; // [mouse button on track] 1 or 3 or settings.scroll_unit or inf
//		double step_track_interval; // 0.3 or 1 or settings.step_button_hold_interval
//		double step_button_hold; // [button > hold per interval] 1 or 3 or settings.scroll_unit
//		double step_button_hold_interval; // 0.3 or 1 or settings.step_button_hold_interval
//		double step_arrow; // [up/down] 1 or 3 or settings.scroll_unit
//		double step_page; // [pageup/pagedown] num_lines_displayed

class ScrollBar : public ComponenetHandler<class CoreScrollBar, EventHostScroll<ScrollBar>> {
public:
//	enum class BarMode : uint8_t {
//		bar_range = 0,
//		bar_fix,
//		fill_high,
//		fill_low,
//	};

public:
	explicit ScrollBar(std::string name);
	explicit ScrollBar(GenerateName_t = {}, const std::string_view type = "scroll-bar");
	explicit ScrollBar(base_ptr core) noexcept;

public:
	void value(double value);
	[[nodiscard]] double value() const noexcept;

	void value_int(int64_t value);
	[[nodiscard]] int64_t value_int() const noexcept;

	void value_max(double value);
	[[nodiscard]] double value_max() const noexcept;

	void value_min(double value);
	[[nodiscard]] double value_min() const noexcept;

	void value_range(double value);
	[[nodiscard]] double value_range() const noexcept;

	void value_step(double value);
	[[nodiscard]] double value_step() const noexcept;

public:
	void make_step(double amount) noexcept;
	void make_scroll(double amount) noexcept;

//	[[nodiscard]] bool is_at_max() const noexcept;
//	[[nodiscard]] bool is_at_min() const noexcept;

public:
	void bar_color(Color value);
	[[nodiscard]] const Color& bar_color() const noexcept;

	void bar_image(Texture2D_view value);
	[[nodiscard]] const Texture2D_view& bar_image() const noexcept;

	void bar_shader(ShaderImage_view value);
	[[nodiscard]] const ShaderImage_view& bar_shader() const noexcept;

	void color(Color value);
	[[nodiscard]] const Color& color() const noexcept;

	void image(Texture2D_view value);
	[[nodiscard]] const Texture2D_view& image() const noexcept;

	void shader(ShaderImage_view value);
	[[nodiscard]] const ShaderImage_view& shader() const noexcept;

public:
	void orientation(Orientation value);
	[[nodiscard]] Orientation orientation() const noexcept;
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
