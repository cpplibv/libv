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
	template <typename T>
	static void access_properties(T& ctx);
//	static ComponentPropertyDescription description;

	struct PS {
		PropertyFFR<Color> color;
		PropertyFFR<ShaderQuad_view> quad_shader;
	} property;

private:
	libv::glr::Mesh mesh{libv::gl::Primitive::Triangles, libv::gl::BufferUsage::StaticDraw};

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
