// Project: libv.ui, File: src/libv/ui/component/image.hpp

#pragma once

// pro
#include <libv/ui/component/detail/component_api.hpp>
#include <libv/ui/property/background.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class Image : public ComponentAPI<Component, Image, class CoreImage, EventHostGeneral> {
public:
	using ComponentAPI::ComponentAPI;
	static constexpr std::string_view component_type = "image";
	[[nodiscard]] static core_ptr create_core(std::string name);
	[[nodiscard]] static bool castable(core_ptr) noexcept;

public:
	void background(Background value);
	[[nodiscard]] const Background& background() const noexcept;
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
