// Project: libv.ui, File: src/libv/ui/component/scroll_area.hpp

#pragma once

// pro
#include <libv/ui/component_system/component_api.hpp>
#include <libv/ui/property/background.hpp>
#include <libv/ui/property/scroll_mode.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

struct EventScrollArea : BaseEvent {
	libv::vec2f old_position;
	libv::vec2f old_size;

	constexpr inline EventScrollArea(libv::vec2f old_position, libv::vec2f old_size) noexcept :
		old_position(old_position),
		old_size(old_size) {}
};

template <typename ComponentT>
struct EventHostScrollArea : EventHostGeneral<ComponentT> {
	BasicEventProxy<ComponentT, EventScrollArea> area{this->owner};
};

// -------------------------------------------------------------------------------------------------

class ScrollArea : public ComponentAPI<Component, ScrollArea, class CoreScrollArea, EventHostScrollArea> {
public:
	using ComponentAPI::ComponentAPI;
	static constexpr std::string_view component_type = "s-area";
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

public:
	void area_position(libv::vec2f value) noexcept;
	[[nodiscard]] libv::vec2f area_position() const noexcept;

	[[nodiscard]] libv::vec2f area_size() const noexcept;
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
