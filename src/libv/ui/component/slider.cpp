// Project: libv.ui, File: src/libv/ui/component/slider.cpp

// hpp
#include <libv/ui/component/slider.hpp>
// libv
#include <libv/math/remap.hpp>
#include <libv/math/snap.hpp>
#include <libv/utility/approx.hpp>
#include <libv/utility/float_equal.hpp>
// std
#include <cmath>
// pro
#include <libv/ui/component/component_core.hpp>
#include <libv/ui/context/context_layout.hpp>
#include <libv/ui/context/context_mouse.hpp>
#include <libv/ui/context/context_render.hpp>
#include <libv/ui/context/context_resource.hpp>
#include <libv/ui/context/context_state.hpp>
#include <libv/ui/context/context_ui.hpp>
#include <libv/ui/event/event_mouse.hpp>
#include <libv/ui/log.hpp>
#include <libv/ui/property_system/property.hpp>
#include <libv/ui/property_system/property_access.hpp>
#include <libv/ui/resource/shader_image.hpp>
#include <libv/ui/resource/texture_2D.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

struct CoreSlider : CoreComponent {
	using base_type = CoreComponent;
	using base_type::base_type;

	// TODO P5: Remove handler()
	[[nodiscard]] inline auto handler() { return Slider{this}; }

public:
	struct BarBounds {
		libv::vec2f position;
		libv::vec2f size;
	};

	enum class DragState : uint8_t {
		idle,
		track,
		bar,
	};

	struct Properties {
		PropertyR<Background> bar_visual;
		PropertyR<Background> background;

		PropertyL1L2<Orientation> orientation;
	} property;

	template <typename T> static void access_properties(T& ctx);

public:
	double value_ = 0.0;
	double value_high_ = 100.0;
	double value_low_ = 0.0;
	double value_range_ = 10.0;
	double value_step_ = 0.0;

public:
	libv::vec2f drag_point;
	DragState drag_state = DragState::idle;

	BarBounds bar_bounds_;

//	double scroll_step_button;
//	double scroll_step_scroll;
//	double scroll_step_track;
//	double scroll_hold_button;
//	double scroll_hold_scroll;
//	double scroll_hold_track;

public:
	void value(double value);
	[[nodiscard]] double value() const noexcept;

	void value_int(int64_t value);
	[[nodiscard]] int64_t value_int() const noexcept;

	void value_high(double value);
	[[nodiscard]] double value_high() const noexcept;

	void value_low(double value);
	[[nodiscard]] double value_low() const noexcept;

	void value_range(double value);
	[[nodiscard]] double value_range() const noexcept;

	void value_step(double value);
	[[nodiscard]] double value_step() const noexcept;

public:
	void make_step(double amount) noexcept;
	void make_scroll(double amount) noexcept;

public:
//	inline auto bar_size() const noexcept;
	inline BarBounds bar_bounds() const noexcept;

public:
	virtual void onMouseButton(const EventMouseButton& event) override;
	virtual void onMouseMovement(const EventMouseMovement& event) override;
	virtual void onMouseScroll(const EventMouseScroll& event) override;

public:
	virtual void doAttach() override;
	virtual void doStyle(StyleAccess& access) override;
	virtual libv::vec3f doLayout1(const ContextLayout1& environment) override;
	virtual void doLayout2(const ContextLayout2& environment) override;
	virtual void doRender(Renderer& r) override;
};

// -------------------------------------------------------------------------------------------------

template <typename T>
void CoreSlider::access_properties(T& ctx) {
	ctx.property(
			[](auto& c) -> auto& { return c.property.bar_visual; },
			Background::color({1, 1, 1, 1}),
			pgr::appearance, pnm::bar_visual,
			"Bar visualization render fragment"
	);

	ctx.property(
			[](auto& c) -> auto& { return c.property.background; },
			Background::none(),
			pgr::appearance, pnm::background,
			"Background"
	);

	ctx.property(
			[](auto& c) -> auto& { return c.property.orientation; },
			Orientation::up,
			pgr::layout, pnm::orientation,
			"Orientation"
	);

	ctx.synthesize(
			[](auto& c, auto v) { c.handler().value(std::move(v)); },
			[](const auto& c) { return c.handler().value(); },
			pgr::behaviour, pnm::value,
			"Current value"
	);
	ctx.synthesize(
			[](auto& c, auto v) { c.handler().value_low(std::move(v)); },
			[](const auto& c) { return c.handler().value_low(); },
			pgr::behaviour, pnm::value_low,
			"Minimum value"
	);
	ctx.synthesize(
			[](auto& c, auto v) { c.handler().value_high(std::move(v)); },
			[](const auto& c) { return c.handler().value_high(); },
			pgr::behaviour, pnm::value_high,
			"Maximum value"
	);
	ctx.synthesize(
			[](auto& c, auto v) { c.handler().value_range(std::move(v)); },
			[](const auto& c) { return c.handler().value_range(); },
			pgr::behaviour, pnm::value_range,
			"Value range covered by the scroll bar"
	);
	ctx.synthesize(
			[](auto& c, auto v) { c.handler().value_step(std::move(v)); },
			[](const auto& c) { return c.handler().value_step(); },
			pgr::behaviour, pnm::value_step,
			"Value step"
	);
}

// -------------------------------------------------------------------------------------------------

namespace {

struct OrientationData {
	int32_t dim_control;
	int32_t dim_secondary;
	bool control_inverted;
	float control_direction;
};

static constexpr OrientationData OrientationTable[] = {
	{1, 0, false, +1}, // Orientation::up;
	{0, 1, false, +1}, // Orientation::right;
	{0, 1,  true, -1}, // Orientation::left;
	{1, 0,  true, -1}, // Orientation::down;
};

} // namespace

// -------------------------------------------------------------------------------------------------

inline CoreSlider::BarBounds CoreSlider::bar_bounds() const noexcept {
	//
	//                    Orientation::up
	//          value == value_low             value == value_high
	//
	//         +->   |..|                     +->   |##|   <-+
	//         |     |..|                     |     |##|     |
	//         |     |..|                     |     |##|     |
	//         |     |..|        value_range' |     |##|     | bar
	//         |     |..|                     |     |##|     |
	//    diff |     |..|                     |     |##|     |
	//         |     |..|   <--- value_high --+->   |##|   <-+-- value
	//         |     |..|                           |..|     |
	//         |     |..|                           |..|     |
	//         |     |..|                           |..|     |
	//         +->   |##|   <-+                     |..|     |
	//         |     |##|     |                     |..|     | diff
	//         |     |##|     |                     |..|     |
	//     bar |     |##|     | value_range'        |..|     |
	//         |     |##|     |                     |..|     |
	//         |     |##|     |                     |..|     |
	// value --+->   |##|   <-+-- value_low ---->   |..|   <-+
	//
	//      value_range' = (value_range) / (value_high - value_low)
	//

	const auto orient = OrientationTable[underlying(property.orientation())];

	const auto value_extent = std::abs(value_high_ - value_low_);

	const auto size_x = static_cast<double>(layout_size2()[orient.dim_control]);
	const auto size_y = static_cast<double>(layout_size2()[orient.dim_secondary]);

	// NOTE: std::clamp is not usable as min should have priority (0 size should yield 0 sized bar)
	const auto size_bar_x = value_extent < value_range_ ?
			size_x :
			std::min(std::max(std::round(value_range_ / value_extent * size_x), 1.0), size_x);
	const auto size_bar_y = size_y;

	const auto position_bar_x =	std::round(value_extent < value_range_ ?
			0.0 :
			orient.control_inverted ?
				libv::remap(value_, value_low_, value_high_, size_x - size_bar_x, 0.0) :
				libv::remap(value_, value_low_, value_high_, 0.0, size_x - size_bar_x)
	);
	const auto position_bar_y = 0.0f;

	BarBounds result;

	result.size[orient.dim_control] = static_cast<float>(size_bar_x);
	result.size[orient.dim_secondary] = static_cast<float>(size_bar_y);

	result.position[orient.dim_control] = static_cast<float>(position_bar_x);
	result.position[orient.dim_secondary] = static_cast<float>(position_bar_y);

	return result;
}

// -------------------------------------------------------------------------------------------------

void CoreSlider::onMouseButton(const EventMouseButton& event) {
	if (event.button == libv::input::MouseButton::Left && event.action == libv::input::Action::press) {
		const auto orient = OrientationTable[underlying(property.orientation())];
		const auto bar = bar_bounds_;
		const auto local_mouse = event.local_position;

		if (libv::vec::within(local_mouse, bar.position, bar.position + bar.size - 1.0f)) {
			drag_state = DragState::bar;
			drag_point = local_mouse - bar.position;
		} else {
			drag_state = DragState::track;
			const auto value_extent = std::abs(value_high_ - value_low_);
			const auto local_drag = std::floor(bar.size[orient.dim_control] * 0.5f);
			const auto local_value = value_extent < value_range_ ?
					value_ :
					libv::remap_clamp<double>(
						local_mouse[orient.dim_control] - local_drag,
						0,
						layout_size2()[orient.dim_control] - bar.size[orient.dim_control],
						orient.control_inverted ? value_high_ : value_low_,
						orient.control_inverted ? value_low_ : value_high_);
			handler().value(local_value);
		}

		ui().mouse.acquire(*this);
		return event.stop_propagation();
	}

	if (event.button == libv::input::MouseButton::Left && event.action == libv::input::Action::release) {
		ui().mouse.release(*this);
		drag_state = DragState::idle;
		return event.stop_propagation();
	}

//	// === TEMP ========================================================================================
//	if (event.button == libv::input::MouseButton::Right && event.action == libv::input::Action::press) {
//		set(property.orientation, Orientation{(underlying(property.orientation()) + 1) % 4});
//		type(property.orientation()).next()
//		return event.stop_propagation();
//	}
//	// === TEMP ========================================================================================
}

void CoreSlider::onMouseMovement(const EventMouseMovement& event) {
	if (event.enter)
		style_state(StyleState::hover, true);

	if (event.leave)
		style_state(StyleState::hover, false);

	// TODO P1: style_state(StyleState::active, false); after the mouse leaves the component area (while it was active): maybe it has to acquire or soft acquire the mouse? so it can track the release (ergo deactivate) event
	//		style_state(StyleState::active, false);

	// TODO P5: Set style to hover if not disabled and updates layout properties in parent


//	if (event.enter) {
////		set(property.bg_color, property.bg_color() + 0.2f);
//		set(property.bar_color, property.bar_color() + 0.2f);
//		// TODO P5: Set style to hover if not disabled and updates layout properties in parent
//	}
//
//	if (event.leave) {
////		set(property.bg_color, property.bg_color() - 0.2f);
//		set(property.bar_color, property.bar_color() - 0.2f);
////		reset(property.bg_color);
////		reset(property.bar_color);
//		// TODO P5: Set style to hover if not disabled and updates layout properties in parent
//	}

	if (drag_state == DragState::idle)
		return;

	const auto orient = OrientationTable[underlying(property.orientation())];
	const auto bar = bar_bounds_; // Cache bar_bounds
	const auto local_mouse = event.local_position;
	const auto value_extent = std::abs(value_high_ - value_low_);

	if (value_extent < value_range_) {
		// Handle special case with oversized (range) bar
		const auto side_a = local_mouse[orient.dim_control] < layout_size2()[orient.dim_control] * 0.5f;
		const auto toward_min = side_a == orient.control_inverted;
		const auto local_value = toward_min ? value_low_ : value_high_;
		handler().value(local_value);
		return event.stop_propagation();
	}

	const auto local_drag = drag_state == DragState::track ?
			std::floor(bar.size[orient.dim_control] * 0.5f) : // DragState::track
			drag_point[orient.dim_control];                   // DragState::bar

	const auto local_value = libv::remap_clamp<double>(
			local_mouse[orient.dim_control] - local_drag,
			0,
			layout_size2()[orient.dim_control] - bar.size[orient.dim_control],
			orient.control_inverted ? value_high_ : value_low_,
			orient.control_inverted ? value_low_ : value_high_);

	handler().value(local_value);
	event.stop_propagation();
}

void CoreSlider::onMouseScroll(const EventMouseScroll& event) {
	const auto orient = OrientationTable[underlying(property.orientation())];
	const auto movement = static_cast<double>(event.scroll_movement.y * orient.control_direction);
	const auto inverse_interval = value_low_ < value_high_ ? 1.0 : -1.0;
	handler().make_scroll(movement * inverse_interval);

	event.stop_propagation();
}

// -------------------------------------------------------------------------------------------------

void CoreSlider::doAttach() {
	drag_state = DragState::idle;

	watchMouse(true);
}

void CoreSlider::doStyle(StyleAccess& access) {
	access.self(*this);
}

libv::vec3f CoreSlider::doLayout1(const ContextLayout1& environment) {
	(void) environment;

// const auto dynamic_size_bar = property.bar_image()->size().cast<float>();
//	const auto dynamic_size_bg = property.bg_image()->size().cast<float>();

//	return {libv::vec::max(dynamic_size_bar, dynamic_size_bg), 0.f};
// 	return {dynamic_size_bar, 0.f};
	return {property.bar_visual().size(), 0.f};
}

void CoreSlider::doLayout2(const ContextLayout2& environment) {
	(void) environment;

	bar_bounds_ = bar_bounds(); // Cache bar_bounds
}

void CoreSlider::doRender(Renderer& r) {
	property.background().render(r, {0, 0}, layout_size2(), *this);

	property.bar_visual().render(r, bar_bounds_.position, bar_bounds_.size, *this);
}

// -------------------------------------------------------------------------------------------------

void CoreSlider::value(double request) {
	if (libv::float_equal(request, value_))
		return;

	if (!libv::math::check_interval(request, value_low_, value_high_))
		log_ui.warn("Attempted to assign value {} outside of accepted interval {} - {}. Clamping value to interval for {}", request, value_low_, value_high_, path());

	const auto var = libv::math::snap_interval(request, value_step_, value_low_, value_high_);

	const auto change = var - value_;
	value_ = var;
	bar_bounds_ = bar_bounds(); // Cache bar_bounds
	flagAuto(Flag::pendingLayout | Flag::pendingRender);
	fire(EventScrollChange{request, change});
}
double CoreSlider::value() const noexcept {
	return value_;
}

void CoreSlider::value_int(int64_t value_) {
	value(static_cast<double>(value_));
}
int64_t CoreSlider::value_int() const noexcept {
	return std::llround(value());
}

void CoreSlider::value_high(double var) {
	if (libv::float_equal(var, value_high_))
		return;

	const auto request = value_;
	value_high_ = var;
	flagAuto(Flag::pendingLayout | Flag::pendingRender);

	if (!libv::math::check_interval(value_, value_low_, value_high_)) {
		log_ui.warn("Assigning value_high {} with current value {} outside of interval {} - {}. Clamping value to the new interval for {}", var, value_, value_low_, value_high_, path());
		const auto new_value = libv::math::clamp_interval(value_, value_low_, value_high_);
		const auto change = new_value - value_;
		value_ = new_value;
		fire(EventScrollChange{request, change});
	}
}
double CoreSlider::value_high() const noexcept {
	return value_high_;
}

void CoreSlider::value_low(double var) {
	if (libv::float_equal(var, value_low_))
		return;

	const auto request = value_;
	value_low_ = var;
	flagAuto(Flag::pendingLayout | Flag::pendingRender);

	if (!libv::math::check_interval(value_, value_low_, value_high_)) {
		log_ui.warn("Assigning value_low {} with current value {} outside of interval {} - {}. Clamping value to the new interval for {}", var, value_, value_low_, value_high_, path());
		const auto new_value = libv::math::clamp_interval(value_, value_low_, value_high_);
		const auto change = new_value - value_;
		value_ = new_value;
		fire(EventScrollChange{request, change});
	}
}
double CoreSlider::value_low() const noexcept {
	return value_low_;
}

void CoreSlider::value_range(double var) {
	if (libv::float_equal(var, value_range_))
		return;

	if (var < 0) {
		log_ui.warn("Attempted to assign negative value: {} as interval. Using the absolute value instead for {}", value_, path());
		var = std::abs(var);
	}

	value_range_ = var;
	flagAuto(Flag::pendingLayout | Flag::pendingRender);
}
double CoreSlider::value_range() const noexcept {
	return value_range_;
}

void CoreSlider::value_step(double var) {
	if (libv::float_equal(var, value_step_))
		return;

	value_step_ = var;
	value(value_);
}
double CoreSlider::value_step() const noexcept {
	return value_step_;
}

// -------------------------------------------------------------------------------------------------

void CoreSlider::make_step(double amount) noexcept {
	// WONT FIX: make_step can overstep the nearest value if the amount is big enough
	//		most noticeable on min/max values that are not aligned to step and current value is min/max.
	//		Fixing this rounding behaviour would result in more oddities than what it would solve
	const auto step = libv::float_equal(value_step_, 0.0) ? 1.0 : value_step_;

	const auto request = value_ + amount * step;
	const auto new_value = libv::math::snap_interval(request, value_step_, value_low_, value_high_);
	const auto change = new_value - value_;
	value_ = new_value;
	flagAuto(Flag::pendingLayout | Flag::pendingRender);
	fire(EventScrollChange{request, change});
}

void CoreSlider::make_scroll(double amount) noexcept {
	make_step(amount);
}

// =================================================================================================

core_ptr Slider::create_core(std::string name) {
	return create_core_ptr<CoreType>(std::move(name));
}

bool Slider::castable(libv::ui::core_ptr core) noexcept {
	return dynamic_cast<CoreType*>(core) != nullptr;
}

// -------------------------------------------------------------------------------------------------

void Slider::value(double request) {
	self().value(request);
}
double Slider::value() const noexcept {
	return self().value();
}
void Slider::value_int(int64_t request) {
	self().value_int(request);
}
int64_t Slider::value_int() const noexcept {
	return self().value_int();
}
void Slider::value_high(double var) {
	self().value_high(var);
}
double Slider::value_high() const noexcept {
	return self().value_high();
}
void Slider::value_low(double var) {
	self().value_low(var);
}
double Slider::value_low() const noexcept {
	return self().value_low();
}
void Slider::value_range(double var) {
	self().value_range(var);
}
double Slider::value_range() const noexcept {
	return self().value_range();
}
void Slider::value_step(double var) {
	self().value_step(var);
}
double Slider::value_step() const noexcept {
	return self().value_step();
}

// -------------------------------------------------------------------------------------------------

void Slider::make_step(double amount) noexcept {
	self().make_step(amount);
}
void Slider::make_scroll(double amount) noexcept {
	self().make_scroll(amount);
}

// -------------------------------------------------------------------------------------------------

void Slider::bar_visual(Background value) {
	AccessProperty::manual(self(), self().property.bar_visual, std::move(value));
}

[[nodiscard]] const Background& Slider::bar_visual() const noexcept {
	return self().property.bar_visual();
}

void Slider::background(Background value) {
	AccessProperty::manual(self(), self().property.background, std::move(value));
}

[[nodiscard]] const Background& Slider::background() const noexcept {
	return self().property.background();
}

// -------------------------------------------------------------------------------------------------

void Slider::orientation(Orientation value) {
	AccessProperty::manual(self(), self().property.orientation, value);
}

Orientation Slider::orientation() const noexcept {
	return self().property.orientation();
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
