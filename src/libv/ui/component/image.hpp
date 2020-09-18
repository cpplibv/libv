// File: image.hpp - Created on 2017.10.22. 10:33 - Author: Vader

#pragma once

// pro
#include <libv/ui/component.hpp>
#include <libv/ui/property/color.hpp>
#include <libv/ui/property/shader_image.hpp>
#include <libv/ui/property/texture_2D.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class Image : public ComponentHandler<class CoreImage, EventHostGeneral<Image>> {
public:
	explicit Image(std::string name);
	explicit Image(GenerateName_t = {}, const std::string_view type = "image");
	explicit Image(core_ptr core) noexcept;

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
