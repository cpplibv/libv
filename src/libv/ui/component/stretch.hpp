// Project: libv.ui, File: src/libv/ui/component/stretch.hpp, Author: Császár Mátyás [Vader]

#pragma once

// pro
#include <libv/ui/component/detail/component_api.hpp>
#include <libv/ui/property/color.hpp>
#include <libv/ui/property/shader_image.hpp>
#include <libv/ui/property/texture_2D.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class Stretch : public ComponentAPI<Component, Stretch, class CoreStretch, EventHostGeneral> {
public:
	using ComponentAPI::ComponentAPI;
	static constexpr std::string_view component_type = "stretch";
	static core_ptr create_core(std::string name);

public:
	void color(Color value);
	[[nodiscard]] const Color& color() const noexcept;

	void image(Texture2D_view value);
	[[nodiscard]] const Texture2D_view& image() const noexcept;

	void shader(ShaderImage_view value);
	[[nodiscard]] const ShaderImage_view& shader() const noexcept;
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
