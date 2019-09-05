// File: image.hpp - Created on 2017.10.22. 10:33 - Author: Vader

#pragma once

// libv
#include <libv/glr/mesh.hpp>
#include <libv/meta/reflection_access.hpp>
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

struct Image : BaseComponent {
private:
	struct PS {
		PropertyColor color;
		PropertyImage image;
		PropertyShaderImage image_shader;

		LIBV_REFLECTION_ACCESS(color);
		LIBV_REFLECTION_ACCESS(image);
		LIBV_REFLECTION_ACCESS(image_shader);
	};

private:
	libv::glr::Mesh mesh{libv::gl::Primitive::Triangles, libv::gl::BufferUsage::StaticDraw};
public:
	libv::ui::PropertySet<PS> properties;

public:
	Image();
	Image(std::string name);
	Image(UnnamedTag, const std::string_view type);
	~Image();

private:
	virtual void doStyle() override;
	virtual void doRender(ContextRender& context) override;
	virtual void doLayout1(const ContextLayout1& environment) override;
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv