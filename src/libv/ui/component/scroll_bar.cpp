// Project: libv.ui, File: src/libv/ui/component/scroll_bar.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <libv/ui/component/scroll_bar.hpp>
// libv
#include <libv/math/remap.hpp>
#include <libv/math/snap.hpp>
#include <libv/utility/approx.hpp>
#include <libv/utility/float_equal.hpp>
// std
#include <cmath>
// pro
#include <libv/ui/context/context_layout.hpp>
#include <libv/ui/context/context_mouse.hpp>
#include <libv/ui/context/context_render.hpp>
#include <libv/ui/context/context_state.hpp>
#include <libv/ui/context/context_style.hpp>
#include <libv/ui/context/context_ui.hpp>
#include <libv/ui/core_component.hpp>
#include <libv/ui/event/event_mouse.hpp>
#include <libv/ui/log.hpp>
#include <libv/ui/property.hpp>
#include <libv/ui/property_access_context.hpp>
#include <libv/ui/shader/shader_image.hpp>
#include <libv/ui/style.hpp>
#include <libv/ui/texture_2D.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class CoreScrollBar : public CoreComponent {
	friend class ScrollBar;
	[[nodiscard]] inline auto handler() { return ScrollBar{this}; }

private:
	template <typename T> static void access_properties(T& ctx);

private:
	struct BarBounds {
		libv::vec2f position;
		libv::vec2f size;
	};

	enum class DragMode : uint8_t {
		idle,
		track,
		bar,
	};

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
	double value_ = 0.0;
	double value_max_ = 100.0;
	double value_min_ = 0.0;
	double value_range_ = 10.0;
	double value_step_ = 0.0;

private:
	libv::vec2f drag_point;
	DragMode drag_mode = DragMode::idle;

	BarBounds bar_bounds_;

//	double scroll_step_button;
//	double scroll_step_scroll;
//	double scroll_step_track;
//	double scroll_hold_button;
//	double scroll_hold_scroll;
//	double scroll_hold_track;

public:
	using CoreComponent::CoreComponent;

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

private:
//	inline auto bar_size() const noexcept;
	inline BarBounds bar_bounds() const noexcept;

private:
	virtual void onMouseButton(const EventMouseButton& event) override;
	virtual void onMouseMovement(const EventMouseMovement& event) override;
	virtual void onMouseScroll(const EventMouseScroll& event) override;

private:
	virtual void doAttach() override;
	virtual void doStyle(ContextStyle& context) override;
	virtual libv::vec3f doLayout1(const ContextLayout1& environment) override;
	virtual void doLayout2(const ContextLayout2& environment) override;
	virtual void doRender(Renderer& r) override;
};

// -------------------------------------------------------------------------------------------------

template <typename T>
void CoreScrollBar::access_properties(T& ctx) {
	ctx.property(
			[](auto& c) -> auto& { return c.property.bar_color; },
			Color(1, 1, 1, 1),
			pgr::appearance, pnm::bar_color,
			"Bar color"
	);
	ctx.property(
			[](auto& c) -> auto& { return c.property.bar_image; },
			[](auto& u) { return u.fallbackTexture2D(); },
			pgr::appearance, pnm::bar_image,
			"Bar image"
	);
	ctx.property(
			[](auto& c) -> auto& { return c.property.bar_shader; },
			[](auto& u) { return u.shaderImage(); },
			pgr::appearance, pnm::bar_shader,
			"Bar shader"
	);

	ctx.property(
			[](auto& c) -> auto& { return c.property.bg_color; },
			Color(1, 1, 1, 1),
			pgr::appearance, pnm::bg_color,
			"Background color"
	);
	ctx.property(
			[](auto& c) -> auto& { return c.property.bg_image; },
			[](auto& u) { return u.fallbackTexture2D(); },
			pgr::appearance, pnm::bg_image,
			"Background image"
	);
	ctx.property(
			[](auto& c) -> auto& { return c.property.bg_shader; },
			[](auto& u) { return u.shaderImage(); },
			pgr::appearance, pnm::bg_shader,
			"Background shader"
	);

	ctx.property(
			[](auto& c) -> auto& { return c.property.orientation; },
			Orientation::BOTTOM_TO_TOP,
			pgr::layout, pnm::orientation,
			"Orientation"
	);

	ctx.synthetize(
			[](auto& c, auto v) { c.handler().value(std::move(v)); },
			[](const auto& c) { return c.handler().value(); },
			pgr::behaviour, pnm::value,
			"Current value"
	);
	ctx.synthetize(
			[](auto& c, auto v) { c.handler().value_min(std::move(v)); },
			[](const auto& c) { return c.handler().value_min(); },
			pgr::behaviour, pnm::value_min,
			"Minimum value"
	);
	ctx.synthetize(
			[](auto& c, auto v) { c.handler().value_max(std::move(v)); },
			[](const auto& c) { return c.handler().value_max(); },
			pgr::behaviour, pnm::value_max,
			"Maximum value"
	);
	ctx.synthetize(
			[](auto& c, auto v) { c.handler().value_range(std::move(v)); },
			[](const auto& c) { return c.handler().value_range(); },
			pgr::behaviour, pnm::value_range,
			"Value range covered by the scroll bar"
	);
	ctx.synthetize(
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
	{1, 0, false, +1}, // BOTTOM_TO_TOP;
	{0, 1, false, +1}, // LEFT_TO_RIGHT;
	{0, 1,  true, -1}, // RIGHT_TO_LEFT;
	{1, 0,  true, -1}, // TOP_TO_BOTTOM;
};

} // namespace

// -------------------------------------------------------------------------------------------------

inline CoreScrollBar::BarBounds CoreScrollBar::bar_bounds() const noexcept {
	//
	//                    Orientation::BOTTOM_TO_TOP
	//          value == value_min             value == value_max
	//
	//         +->   |..|                     +->   |##|   <-+
	//         |     |..|                     |     |##|     |
	//         |     |..|                     |     |##|     |
	//         |     |..|        value_range' |     |##|     | bar
	//         |     |..|                     |     |##|     |
	//    diff |     |..|                     |     |##|     |
	//         |     |..|   <---- value_max --+->   |##|   <-+-- value
	//         |     |..|                           |..|     |
	//         |     |..|                           |..|     |
	//         |     |..|                           |..|     |
	//         +->   |##|   <-+                     |..|     |
	//         |     |##|     |                     |..|     | diff
	//         |     |##|     |                     |..|     |
	//     bar |     |##|     | value_range'        |..|     |
	//         |     |##|     |                     |..|     |
	//         |     |##|     |                     |..|     |
	// value --+->   |##|   <-+-- value_min ---->   |..|   <-+
	//
	//      value_range' = (value_range) / (value_max - value_min)
	//

	const auto orient = OrientationTable[libv::to_value(property.orientation())];

	const auto value_extent = std::abs(value_max_ - value_min_);

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
				libv::remap(value_, value_min_, value_max_, size_x - size_bar_x, 0.0) :
				libv::remap(value_, value_min_, value_max_, 0.0, size_x - size_bar_x)
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

void CoreScrollBar::onMouseButton(const EventMouseButton& event) {
	if (event.button == libv::input::MouseButton::Left && event.action == libv::input::Action::press) {
		const auto orient = OrientationTable[libv::to_value(property.orientation())];
		const auto bar = bar_bounds_;
		const auto local_mouse = event.local_position;

		if (libv::vec::within(local_mouse, bar.position, bar.position + bar.size - 1.0f)) {
			drag_mode = DragMode::bar;
			drag_point = local_mouse - bar.position;
		} else {
			drag_mode = DragMode::track;
			const auto value_extent = std::abs(value_max_ - value_min_);
			const auto local_drag = std::floor(bar.size[orient.dim_control] * 0.5f);
			const auto local_value = value_extent < value_range_ ?
					value_ :
					libv::remap_clamp<double>(
						local_mouse[orient.dim_control] - local_drag,
						0,
						layout_size2()[orient.dim_control] - bar.size[orient.dim_control],
						orient.control_inverted ? value_max_ : value_min_,
						orient.control_inverted ? value_min_ : value_max_);
			handler().value(local_value);
		}

		context().mouse.acquire(*this);
		return event.stop_propagation();
	}

	if (event.button == libv::input::MouseButton::Left && event.action == libv::input::Action::release) {
		context().mouse.release(*this);
		drag_mode = DragMode::idle;
		return event.stop_propagation();
	}

	// === TEMP ========================================================================================
	if (event.button == libv::input::MouseButton::Right && event.action == libv::input::Action::press) {
		set(property.orientation, Orientation{(libv::to_value(property.orientation()) + 1) % 4});
		return event.stop_propagation();
	}
	// === TEMP ========================================================================================
}

void CoreScrollBar::onMouseMovement(const EventMouseMovement& event) {
	if (event.enter) {
		set(property.bg_color, property.bg_color() + 0.2f);
		set(property.bar_color, property.bar_color() + 0.2f);
		// TODO P5: Set style to hover if not disabled and updates layout properties in parent
	}

	if (event.leave) {
		set(property.bg_color, property.bg_color() - 0.2f);
		set(property.bar_color, property.bar_color() - 0.2f);
//		reset(property.bg_color);
//		reset(property.bar_color);
		// TODO P5: Set style to hover if not disabled and updates layout properties in parent
	}

	if (drag_mode == DragMode::idle)
		return;

	const auto orient = OrientationTable[libv::to_value(property.orientation())];
	const auto bar = bar_bounds_; // Cache bar_bounds
	const auto local_mouse = event.local_position;
	const auto value_extent = std::abs(value_max_ - value_min_);

	if (value_extent < value_range_) {
		// Handle special case with oversized (range) bar
		const auto side_a = local_mouse[orient.dim_control] < layout_size2()[orient.dim_control] * 0.5f;
		const auto toward_min = side_a == orient.control_inverted;
		const auto local_value = toward_min ? value_min_ : value_max_;
		handler().value(local_value);
		return event.stop_propagation();
	}

	const auto local_drag = drag_mode == DragMode::track ?
			std::floor(bar.size[orient.dim_control] * 0.5f) : // DragMode::track
			drag_point[orient.dim_control];                   // DragMode::bar

	const auto local_value = libv::remap_clamp<double>(
			local_mouse[orient.dim_control] - local_drag,
			0,
			layout_size2()[orient.dim_control] - bar.size[orient.dim_control],
			orient.control_inverted ? value_max_ : value_min_,
			orient.control_inverted ? value_min_ : value_max_);

	handler().value(local_value);
	event.stop_propagation();
}

void CoreScrollBar::onMouseScroll(const EventMouseScroll& event) {
	const auto orient = OrientationTable[libv::to_value(property.orientation())];
	const auto movement = static_cast<double>(event.scroll_movement.y * orient.control_direction);
	const auto inverse_interval = value_min_ < value_max_ ? 1.0 : -1.0;
	handler().make_scroll(movement * inverse_interval);

	event.stop_propagation();
}

// -------------------------------------------------------------------------------------------------

void CoreScrollBar::doAttach() {
	drag_mode = DragMode::idle;

	watchMouse(true);
}

void CoreScrollBar::doStyle(ContextStyle& ctx) {
	PropertyAccessContext<CoreScrollBar> setter{*this, ctx.component, ctx.style, context()};
	access_properties(setter);
	CoreComponent::access_properties(setter);
}

libv::vec3f CoreScrollBar::doLayout1(const ContextLayout1& environment) {
	(void) environment;

	const auto dynamic_size_bar = property.bar_image()->size().cast<float>();
	const auto dynamic_size_bg = property.bg_image()->size().cast<float>();

	return {libv::vec::max(dynamic_size_bar, dynamic_size_bg), 0.f};
}

void CoreScrollBar::doLayout2(const ContextLayout2& environment) {
	(void) environment;

	bar_bounds_ = bar_bounds(); // Cache bar_bounds
}

void CoreScrollBar::doRender(Renderer& r) {
	r.texture_2D({0, 0}, layout_size2(), {0, 0}, {1, 1},
			property.bg_color(),
			property.bg_image(),
			property.bg_shader());

	r.texture_2D(bar_bounds_.position, bar_bounds_.size, {0, 0}, {1, 1},
			property.bar_color(),
			property.bar_image(),
			property.bar_shader());
}

// -------------------------------------------------------------------------------------------------

void CoreScrollBar::value(double request) {
	if (libv::float_equal(request, value_))
		return;

	if (!libv::math::check_interval(request, value_min_, value_max_))
		log_ui.warn("Attempted to assign value {} outside of accepted interval {} - {}. Clamping value to interval for {}", request, value_min_, value_max_, path());

	const auto var = libv::math::snap_interval(request, value_step_, value_min_, value_max_);

	const auto change = var - value_;
	value_ = var;
	bar_bounds_ = bar_bounds(); // Cache bar_bounds
	flagAuto(Flag::pendingLayout | Flag::pendingRender);
	fire(EventScrollChange{request, change});
}
double CoreScrollBar::value() const noexcept {
	return value_;
}

void CoreScrollBar::value_int(int64_t value_) {
	value(static_cast<double>(value_));
}
int64_t CoreScrollBar::value_int() const noexcept {
	return std::llround(value());
}

void CoreScrollBar::value_max(double var) {
	if (libv::float_equal(var, value_max_))
		return;

	const auto request = value_;
	value_max_ = var;
	flagAuto(Flag::pendingLayout | Flag::pendingRender);

	if (!libv::math::check_interval(value_, value_min_, value_max_)) {
		log_ui.warn("Assigning value_max {} with current value {} outside of interval {} - {}. Clamping value to the new interval for {}", var, value_, value_min_, value_max_, path());
		const auto new_value = libv::math::clamp_interval(value_, value_min_, value_max_);
		const auto change = new_value - value_;
		value_ = new_value;
		fire(EventScrollChange{request, change});
	}
}
double CoreScrollBar::value_max() const noexcept {
	return value_max_;
}

void CoreScrollBar::value_min(double var) {
	if (libv::float_equal(var, value_min_))
		return;

	const auto request = value_;
	value_min_ = var;
	flagAuto(Flag::pendingLayout | Flag::pendingRender);

	if (!libv::math::check_interval(value_, value_min_, value_max_)) {
		log_ui.warn("Assigning value_min {} with current value {} outside of interval {} - {}. Clamping value to the new interval for {}", var, value_, value_min_, value_max_, path());
		const auto new_value = libv::math::clamp_interval(value_, value_min_, value_max_);
		const auto change = new_value - value_;
		value_ = new_value;
		fire(EventScrollChange{request, change});
	}
}
double CoreScrollBar::value_min() const noexcept {
	return value_min_;
}

void CoreScrollBar::value_range(double var) {
	if (libv::float_equal(var, value_range_))
		return;

	if (var < 0) {
		log_ui.warn("Attempted to assign negative value: {} as interval. Using the absolute value instead for {}", value_, path());
		var = std::abs(var);
	}

	value_range_ = var;
	flagAuto(Flag::pendingLayout | Flag::pendingRender);
}
double CoreScrollBar::value_range() const noexcept {
	return value_range_;
}

void CoreScrollBar::value_step(double var) {
	if (libv::float_equal(var, value_step_))
		return;

	value_step_ = var;
	value(value_);
}
double CoreScrollBar::value_step() const noexcept {
	return value_step_;
}

// -------------------------------------------------------------------------------------------------

void CoreScrollBar::make_step(double amount) noexcept {
	// WONT FIX: make_step can overstep the nearest value if the amount is big enough
	//		most noticeable on min/max values that are not aligned to step and current value is min/max.
	//		Fixing this rounding behaviour would result in more oddities than what it would solve
	const auto step = libv::float_equal(value_step_, 0.0) ? 1.0 : value_step_;

	const auto request = value_;
	const auto new_value = libv::math::snap_interval(value_ + amount * step, value_step_, value_min_, value_max_);
	const auto change = new_value - value_;
	value_ = new_value;
	flagAuto(Flag::pendingLayout | Flag::pendingRender);
	fire(EventScrollChange{request, change});
}

void CoreScrollBar::make_scroll(double amount) noexcept {
	make_step(amount);
}

// =================================================================================================

ScrollBar::ScrollBar(std::string name) :
		ComponentHandler<CoreScrollBar, EventHostScroll<ScrollBar>>(std::move(name)) { }

ScrollBar::ScrollBar(GenerateName_t gen, const std::string_view type) :
		ComponentHandler<CoreScrollBar, EventHostScroll<ScrollBar>>(gen, type) { }

ScrollBar::ScrollBar(core_ptr core) noexcept :
		ComponentHandler<CoreScrollBar, EventHostScroll<ScrollBar>>(core) { }

// -------------------------------------------------------------------------------------------------

void ScrollBar::value(double request) {
	self().value(request);
}
double ScrollBar::value() const noexcept {
	return self().value();
}
void ScrollBar::value_int(int64_t request) {
	self().value_int(request);
}
int64_t ScrollBar::value_int() const noexcept {
	return self().value_int();
}
void ScrollBar::value_max(double var) {
	self().value_max(var);
}
double ScrollBar::value_max() const noexcept {
	return self().value_max();
}
void ScrollBar::value_min(double var) {
	self().value_min(var);
}
double ScrollBar::value_min() const noexcept {
	return self().value_min();
}
void ScrollBar::value_range(double var) {
	self().value_range(var);
}
double ScrollBar::value_range() const noexcept {
	return self().value_range();
}
void ScrollBar::value_step(double var) {
	self().value_step(var);
}
double ScrollBar::value_step() const noexcept {
	return self().value_step();
}

// -------------------------------------------------------------------------------------------------

void ScrollBar::make_step(double amount) noexcept {
	self().make_step(amount);
}
void ScrollBar::make_scroll(double amount) noexcept {
	self().make_scroll(amount);
}

// -------------------------------------------------------------------------------------------------

void ScrollBar::bar_color(Color value) {
	AccessProperty::manual(self(), self().property.bar_color, value);
}

const Color& ScrollBar::bar_color() const noexcept {
	return self().property.bar_color();
}

void ScrollBar::bar_image(Texture2D_view value) {
	AccessProperty::manual(self(), self().property.bar_image, std::move(value));
}

const Texture2D_view& ScrollBar::bar_image() const noexcept {
	return self().property.bar_image();
}

void ScrollBar::bar_shader(ShaderImage_view value) {
	AccessProperty::manual(self(), self().property.bar_shader, std::move(value));
}

const ShaderImage_view& ScrollBar::bar_shader() const noexcept {
	return self().property.bar_shader();
}

void ScrollBar::color(Color value) {
	AccessProperty::manual(self(), self().property.bg_color, value);
}

const Color& ScrollBar::color() const noexcept {
	return self().property.bg_color();
}

void ScrollBar::image(Texture2D_view value) {
	AccessProperty::manual(self(), self().property.bg_image, std::move(value));
}

const Texture2D_view& ScrollBar::image() const noexcept {
	return self().property.bg_image();
}

void ScrollBar::shader(ShaderImage_view value) {
	AccessProperty::manual(self(), self().property.bg_shader, std::move(value));
}

const ShaderImage_view& ScrollBar::shader() const noexcept {
	return self().property.bg_shader();
}

// -------------------------------------------------------------------------------------------------

void ScrollBar::orientation(Orientation value) {
	AccessProperty::manual(self(), self().property.orientation, value);
}

Orientation ScrollBar::orientation() const noexcept {
	return self().property.orientation();
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
