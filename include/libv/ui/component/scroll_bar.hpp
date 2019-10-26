// File: label.hpp - Created on 2017.10.22. 07:12 - Author: Vader

#pragma once

// libv
#include <libv/glr/mesh.hpp>
#include <libv/input/input.hpp>
// std
#include <functional>
#include <string>
#include <string_view>
// pro
#include <libv/ui/base_component.hpp>
//#include <libv/ui/event/event_mouse.hpp>
#include <libv/ui/property.hpp>


namespace libv {
namespace ui {

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

class ScrollBar : public BaseComponent {
private:
	enum class DragMode : uint8_t {
		idle,
		track,
		bar,
	};

//	enum class BarMode : uint8_t {
//		bar_fix,
//		bar_range,
//		fill_high,
//		fill_low,
//	};

public:
	struct EChange {
		ScrollBar& component;
		double change;
	};

private:
	template <typename T>
	static void access_properties(T& ctx);

	struct Properties {
		PropertyR<Color> bar_color;
		PropertyL<Texture2D_view> bar_image;
		PropertyR<ShaderImage_view> bar_shader;

		PropertyR<Color> bg_color;
		PropertyL<Texture2D_view> bg_image;
		PropertyR<ShaderImage_view> bg_shader;

		PropertyL<Orientation> orientation;
	} property;

private:
	libv::glr::Mesh bar_mesh{libv::gl::Primitive::Triangles, libv::gl::BufferUsage::StaticDraw};
	libv::glr::Mesh bg_mesh{libv::gl::Primitive::Triangles, libv::gl::BufferUsage::StaticDraw};

private:
	double value_ = 0.0;
	double value_max_ = 100.0;
	double value_min_ = 0.0;
	double value_range_ = 10.0;
	double value_step_ = 0.0;

private:
	libv::vec2f drag_point;
	DragMode drag_mode = DragMode::idle;

//	double scroll_step_button;
//	double scroll_step_scroll;
//	double scroll_step_track;
//	double scroll_hold_button;
//	double scroll_hold_scroll;
//	double scroll_hold_track;

public:
	explicit ScrollBar(BaseComponent& parent);
	ScrollBar(BaseComponent& parent, std::string name);
	ScrollBar(BaseComponent& parent, GenerateName_t, const std::string_view type);
	~ScrollBar();

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

	void make_step(double amount) noexcept;
	void make_scroll(double amount) noexcept;

//	[[nodiscard]] bool is_at_max() const noexcept;
//	[[nodiscard]] bool is_at_min() const noexcept;

private:
//	inline auto bar_size() const noexcept;
	inline auto bar_bounds() const noexcept;

public:
	template <typename F>
	inline void event_change(libv::observer_ptr<BaseComponent> slot, F&& func) {
		connect<EChange>(slot, std::forward<F>(func));
	}

private:
	virtual bool onMouseButton(const EventMouseButton& event) override;
	virtual bool onMouseMovement(const EventMouseMovement& event) override;
	virtual bool onMouseScroll(const EventMouseScroll& event) override;

private:
	virtual void doAttach() override;
	virtual void doStyle(ContextStyle& ctx) override;
	virtual void doLayout1(const ContextLayout1& environment) override;
	virtual void doLayout2(const ContextLayout2& environment) override;
	virtual void doRender(ContextRender& context) override;
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
