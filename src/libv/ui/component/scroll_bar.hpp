// Project: libv.ui, File: src/libv/ui/component/scroll_bar.hpp, Author: Császár Mátyás [Vader]

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

class ScrollBar : public ComponentHandler<class CoreScrollBar, EventHostScroll<ScrollBar>> {
public:
//	enum class BarMode : uint8_t {
//		bar_range = 0,
//		bar_fix,
//		fill_high,
//		fill_low,
//	};

public:
	explicit ScrollBar(std::string name);
	explicit ScrollBar(GenerateName_t = {}, const std::string_view type = "s-bar");
	explicit ScrollBar(core_ptr core) noexcept;

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
