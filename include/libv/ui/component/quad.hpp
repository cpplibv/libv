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


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

struct Quad : BaseComponent {
private:
	template <typename T>
	static void access_properties(T& ctx);
//	static ComponentPropertyDescription description;

	struct Properties {
		PropertyR<Color> color;
		PropertyR<ShaderQuad_view> quad_shader;
	} property;

private:
	libv::glr::Mesh mesh{libv::gl::Primitive::Triangles, libv::gl::BufferUsage::StaticDraw};

public:
	explicit Quad(BaseComponent& parent);
	Quad(BaseComponent& parent, std::string name);
	Quad(BaseComponent& parent, GenerateName_t, const std::string_view type);
	~Quad();

private:
	virtual void doStyle(ContextStyle& ctx) override;
	virtual void doRender(ContextRender& context) override;
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
