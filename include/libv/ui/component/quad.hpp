// File: quad.hpp - Created on 2017.10.22. 10:33 - Author: Vader

#pragma once

// libv
#include <libv/glr/mesh.hpp>
// std
#include <string>
#include <string_view>
// pro
#include <libv/ui/base_component.hpp>
#include <libv/ui/property.hpp>
#include <libv/ui/property_set.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

struct Quad : BaseComponent {
private:
	struct PS {
		static constexpr Flag_t::value_type  R = (Flag::pendingRender).value();

		Property<Color,           R, pnm::color> color;
		Property<ShaderQuad_view, R, pnm::quad_shader> quad_shader;

		template <typename T>
		void access(T& ctx) {
			ctx(color, "Color");
			ctx(quad_shader, "Quad shader");
		}
	};

private:
	libv::glr::Mesh mesh{libv::gl::Primitive::Triangles, libv::gl::BufferUsage::StaticDraw};
public:
	libv::ui::PropertySet<PS> property;

public:
	Quad();
	Quad(std::string name);
	Quad(UnnamedTag_t, const std::string_view type);
	~Quad();

private:
	virtual void doStyle(ContextStyle& ctx) override;
	virtual void doRender(ContextRender& context) override;
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
