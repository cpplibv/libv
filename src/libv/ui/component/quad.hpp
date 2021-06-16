// Project: libv.ui, File: src/libv/ui/component/quad.hpp, Author: Császár Mátyás [Vader]

#pragma once

// pro
#include <libv/ui/component/detail/component_api.hpp>
#include <libv/ui/property/color.hpp>
#include <libv/ui/property/shader_quad.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class Quad : public ComponentAPI<Component, Quad, class CoreQuad, EventHostGeneral> {
public:
	using ComponentAPI::ComponentAPI;
	static constexpr std::string_view component_type = "quad";
	static core_ptr create_core(std::string name);

public:
	void color(Color value);
	[[nodiscard]] const Color& color() const noexcept;

	void shader(ShaderQuad_view value);
	[[nodiscard]] const ShaderQuad_view& shader() const noexcept;
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
