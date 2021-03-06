// Project: libv.ui, File: src/libv/ui/component/quad.hpp, Author: Császár Mátyás [Vader]

#pragma once

// pro
#include <libv/ui/component.hpp>
#include <libv/ui/property/color.hpp>
#include <libv/ui/property/shader_quad.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class Quad : public ComponentHandler<class CoreQuad, EventHostGeneral<Quad>> {
public:
	explicit Quad(std::string name);
	explicit Quad(GenerateName_t = {}, const std::string_view type = "quad");
	explicit Quad(core_ptr core) noexcept;

public:
	void color(Color value);
	[[nodiscard]] const Color& color() const noexcept;

	void shader(ShaderQuad_view value);
	[[nodiscard]] const ShaderQuad_view& shader() const noexcept;
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
