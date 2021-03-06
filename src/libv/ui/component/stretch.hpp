// Project: libv.ui, File: src/libv/ui/component/stretch.hpp, Author: Császár Mátyás [Vader]

#pragma once

// pro
#include <libv/ui/component.hpp>
#include <libv/ui/property/color.hpp>
#include <libv/ui/property/shader_image.hpp>
#include <libv/ui/property/texture_2D.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class Stretch : public ComponentHandler<class CoreStretch, EventHostGeneral<Stretch>> {
public:
	explicit Stretch(std::string name);
	explicit Stretch(GenerateName_t = {}, const std::string_view type = "image");
	explicit Stretch(core_ptr core) noexcept;

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
