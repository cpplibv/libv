// File: quad.hpp - Created on 2017.10.22. 10:33 - Author: Vader

#pragma once

// libv
#include <libv/glr/mesh.hpp>
#include <libv/meta/reflection_access.hpp>
// std
#include <string>
#include <string_view>
// pro
#include <libv/ui/component_base.hpp>
#include <libv/ui/property.hpp>
#include <libv/ui/property_set.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

struct Quad : ComponentBase {
private:
	struct PS {
		PropertyColor color;
		PropertyShaderQuad quad_shader;

		LIBV_REFLECTION_ACCESS(color);
		LIBV_REFLECTION_ACCESS(quad_shader);
	};

private:
	libv::glr::Mesh mesh{libv::gl::Primitive::Triangles, libv::gl::BufferUsage::StaticDraw};
public:
	libv::ui::PropertySet<PS> properties;

public:
	Quad();
	Quad(std::string name);
	Quad(UnnamedTag, const std::string_view type);
	~Quad();

private:
	virtual void doStyle() override;
	virtual void doRender(ContextRender& context) override;
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
