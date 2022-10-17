// Project: libv.ui, File: src/libv/ui/overlay_zoom.lpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
#include <libv/glr/fwd.hpp>
#include <libv/math/vec.hpp>
// pro
#include <libv/ui/component/detail/component_api.hpp>
#include <libv/ui/event_host.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

enum class OverlayZoomMode {
	disabled,
	control,
	view,
};

class OverlayZoom : public ComponentAPI<Component, OverlayZoom, class CoreOverlayZoom, EventHostGeneral> {
public:
	using ComponentAPI::ComponentAPI;
	static constexpr std::string_view component_type = "overlay-zoom";
	[[nodiscard]] static core_ptr create_core(std::string name);
	[[nodiscard]] static bool castable(core_ptr) noexcept;

public:
	void control();
	void view();
	void disable();

public:
	[[nodiscard]] libv::vec2f screen_BL() const;
	[[nodiscard]] libv::vec2f screen_TR() const;

public:
	void postRender(libv::glr::Queue& gl);
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
