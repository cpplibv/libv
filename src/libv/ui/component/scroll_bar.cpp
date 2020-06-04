// File: label.cpp - Created on 2017.11.05. 07:52 - Author: Vader

// hpp
#include <libv/ui/component/scroll_bar.hpp>
// libv
#include <libv/glr/mesh.hpp>
#include <libv/glr/queue.hpp>
#include <libv/math/remap.hpp>
#include <libv/math/snap.hpp>
#include <libv/utility/approx.hpp>
#include <libv/utility/float_equal.hpp>
// std
#include <cmath>
// pro
#include <libv/ui/context_layout.hpp>
#include <libv/ui/context_mouse.hpp>
#include <libv/ui/context_render.hpp>
#include <libv/ui/context_state.hpp>
#include <libv/ui/context_style.hpp>
#include <libv/ui/context_ui.hpp>
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

class CoreScrollBar : public BaseComponent {
	friend class ScrollBar;
	[[nodiscard]] inline auto handler() { return ScrollBar{this}; }

private:
	template <typename T> static void access_properties(T& ctx);

private:
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
	using BaseComponent::BaseComponent;

private:
//	inline auto bar_size() const noexcept;
	inline auto bar_bounds() const noexcept;

private:
	virtual void onMouseButton(const EventMouseButton& event) override;
	virtual void onMouseMovement(const EventMouseMovement& event) override;
	virtual void onMouseScroll(const EventMouseScroll& event) override;

private:
	virtual void doAttach() override;
	virtual void doStyle(ContextStyle& ctx) override;
	virtual void doLayout1(const ContextLayout1& environment) override;
	virtual void doLayout2(const ContextLayout2& environment) override;
	virtual void doRender(ContextRender& context) override;
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

inline auto CoreScrollBar::bar_bounds() const noexcept {
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

	const auto size_x = static_cast<double>(size2()[orient.dim_control]);
	const auto size_y = static_cast<double>(size2()[orient.dim_secondary]);

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

	struct Bounds {
		libv::vec2f position;
		libv::vec2f size;
	} bar;

	bar.size[orient.dim_control] = static_cast<float>(size_bar_x);
	bar.size[orient.dim_secondary] = static_cast<float>(size_bar_y);

	bar.position[orient.dim_control] = static_cast<float>(position_bar_x);
	bar.position[orient.dim_secondary] = static_cast<float>(position_bar_y);

	return bar;
}

// -------------------------------------------------------------------------------------------------

void CoreScrollBar::onMouseButton(const EventMouseButton& event) {
	if (event.button == libv::input::MouseButton::Left && event.action == libv::input::Action::press) {
		const auto orient = OrientationTable[libv::to_value(property.orientation())];
		const auto bar = bar_bounds();
		// TODO P0: libv.ui: correct local mouse position;
		const auto local_mouse = context().state.mouse_position() - position2();

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
						size2()[orient.dim_control] - bar.size[orient.dim_control],
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
		reset(property.bg_color);
		reset(property.bar_color);
		// TODO P5: Set style to hover if not disabled and updates layout properties in parent
	}

	if (drag_mode == DragMode::idle)
		return;

	const auto orient = OrientationTable[libv::to_value(property.orientation())];
	const auto bar = bar_bounds();
	// TODO P0: libv.ui: correct local mouse position;
	const auto local_mouse = event.mouse_position - position2();
	const auto value_extent = std::abs(value_max_ - value_min_);

	if (value_extent < value_range_) {
		// Handle special case with oversized (range) bar
		const auto side_a = local_mouse[orient.dim_control] < size2()[orient.dim_control] * 0.5f;
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
			size2()[orient.dim_control] - bar.size[orient.dim_control],
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

	watchMouse(Flag::mask_watchMouse);
}

void CoreScrollBar::doStyle(ContextStyle& ctx) {
	PropertyAccessContext<CoreScrollBar> setter{*this, ctx.component, ctx.style, context()};
	access_properties(setter);
}

void CoreScrollBar::doLayout1(const ContextLayout1& environment) {
	(void) environment;

	const auto bar = property.bar_image()->size().cast<float>();
	const auto bg = property.bg_image()->size().cast<float>();

	AccessLayout::lastDynamic(*this) = {libv::vec::max(bar, bg), 0.f};
}

void CoreScrollBar::doLayout2(const ContextLayout2& environment) {
	(void) environment;

	{
		bg_mesh.clear();

		auto pos = bg_mesh.attribute(attribute_position);
		auto tex = bg_mesh.attribute(attribute_texture0);
		auto index = bg_mesh.index();

		pos(0, 0, 0);
		pos(size().x, 0, 0);
		pos(size().x, size().y, 0);
		pos(0, size().y, 0);

		tex(0, 0);
		tex(1, 0);
		tex(1, 1);
		tex(0, 1);

		index.quad(0, 1, 2, 3);
	}
	{
		bar_mesh.clear();

		auto pos = bar_mesh.attribute(attribute_position);
		auto tex = bar_mesh.attribute(attribute_texture0);
		auto index = bar_mesh.index();

		const auto bar = bar_bounds();

		pos(bar.position.x, bar.position.y, 0);
		pos(bar.position.x + bar.size.x, bar.position.y, 0);
		pos(bar.position.x + bar.size.x, bar.position.y + bar.size.y, 0);
		pos(bar.position.x, bar.position.y + bar.size.y, 0);

		tex(0, 0);
		tex(1, 0);
		tex(1, 1);
		tex(0, 1);

		index.quad(0, 1, 2, 3);
	}
}

void CoreScrollBar::doRender(ContextRender& ctx) {
	const auto guard_m = ctx.gl.model.push_guard();
 	ctx.gl.model.translate(position());

	{
		ctx.gl.program(*property.bg_shader());
		ctx.gl.texture(property.bg_image()->texture(), property.bg_shader()->textureChannel);
		ctx.gl.uniform(property.bg_shader()->uniform_color, property.bg_color());
		ctx.gl.uniform(property.bg_shader()->uniform_MVPmat, ctx.gl.mvp());
		ctx.gl.render(bg_mesh);
	} {
		ctx.gl.program(*property.bar_shader());
		ctx.gl.texture(property.bar_image()->texture(), property.bar_shader()->textureChannel);
		ctx.gl.uniform(property.bar_shader()->uniform_color, property.bar_color());
		ctx.gl.uniform(property.bar_shader()->uniform_MVPmat, ctx.gl.mvp());
		ctx.gl.render(bar_mesh);
	}
}

// -------------------------------------------------------------------------------------------------

ScrollBar::ScrollBar(std::string name) :
	ComponenetHandler<CoreScrollBar, EventHostScroll<ScrollBar>>(std::move(name)) { }

ScrollBar::ScrollBar(GenerateName_t gen, const std::string_view type) :
	ComponenetHandler<CoreScrollBar, EventHostScroll<ScrollBar>>(gen, type) { }

ScrollBar::ScrollBar(base_ptr core) noexcept :
	ComponenetHandler<CoreScrollBar, EventHostScroll<ScrollBar>>(core) { }

// -------------------------------------------------------------------------------------------------

void ScrollBar::value(double var) {
	if (!libv::math::check_interval(var, self().value_min_, self().value_max_))
		log_ui.warn("Attempted to assign value {} outside of accepted interval {} - {}. Clamping value to interval for {}", var, self().value_min_, self().value_max_, path());

	var = libv::math::snap_interval(var, self().value_step_, self().value_min_, self().value_max_);

	const auto change = var - self().value_;
	self().value_ = var;
	self().flagAuto(Flag::pendingLayout | Flag::pendingRender);
	self().fire(EventScrollChange{change});
}
double ScrollBar::value() const noexcept {
	return self().value_;
}

void ScrollBar::value_int(int64_t value_) {
	value(static_cast<double>(value_));
}
int64_t ScrollBar::value_int() const noexcept {
	return std::llround(value());
}

void ScrollBar::value_max(double var) {
	self().value_max_ = var;
	self().flagAuto(Flag::pendingLayout | Flag::pendingRender);

	if (!libv::math::check_interval(self().value_, self().value_min_, self().value_max_)) {
		log_ui.warn("Assigning value_max {} with current value {} outside of interval {} - {}. Clamping value to the new interval for {}", var, self().value_, self().value_min_, self().value_max_, path());
		const auto new_value = libv::math::clamp_interval(self().value_, self().value_min_, self().value_max_);
		const auto change = new_value - self().value_;
		self().value_ = new_value;
		self().fire(EventScrollChange{change});
	}
}
double ScrollBar::value_max() const noexcept {
	return self().value_max_;
}

void ScrollBar::value_min(double var) {
	self().value_min_ = var;
	self().flagAuto(Flag::pendingLayout | Flag::pendingRender);

	if (!libv::math::check_interval(self().value_, self().value_min_, self().value_max_)) {
		log_ui.warn("Assigning value_min {} with current value {} outside of interval {} - {}. Clamping value to the new interval for {}", var, self().value_, self().value_min_, self().value_max_, path());
		const auto new_value = libv::math::clamp_interval(self().value_, self().value_min_, self().value_max_);
		const auto change = new_value - self().value_;
		self().value_ = new_value;
		self().fire(EventScrollChange{change});
	}
}
double ScrollBar::value_min() const noexcept {
	return self().value_min_;
}

void ScrollBar::value_range(double var) {
	if (var < 0) {
		log_ui.warn("Attempted to assign negative value: {} as interval. Using the absolute value instead for {}", self().value_, path());
		var = std::abs(var);
	}

	self().value_range_ = var;
	self().flagAuto(Flag::pendingLayout | Flag::pendingRender);
}
double ScrollBar::value_range() const noexcept {
	return self().value_range_;
}

void ScrollBar::value_step(double var) {
	self().value_step_ = var;
	value(self().value_);
}
double ScrollBar::value_step() const noexcept {
	return self().value_step_;
}

// -------------------------------------------------------------------------------------------------

void ScrollBar::make_step(double amount) noexcept {
	// WONT FIX: make_step can overstep the nearest value if the amount is big enough
	//		most noticeable on min/max values that are not aligned to step and current value is min/max.
	//		Fixing this rounding behaviour would result in more oddities than what it would solve
	const auto step = libv::float_equal(self().value_step_, 0.0) ? 1.0 : self().value_step_;

	const auto new_value = libv::math::snap_interval(self().value_ + amount * step, self().value_step_, self().value_min_, self().value_max_);
	const auto change = new_value - self().value_;
	self().value_ = new_value;
	self().flagAuto(Flag::pendingLayout | Flag::pendingRender);
	self().fire(EventScrollChange{change});
}

void ScrollBar::make_scroll(double amount) noexcept {
	make_step(amount);
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
