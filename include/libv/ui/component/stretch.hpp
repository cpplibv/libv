// File: stretch.hpp - Created on 2017.10.22. 10:33 - Author: Vader

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

struct Stretch : BaseComponent {
private:
	struct PS {
		static constexpr Flag_t::value_type L  = (Flag::pendingLayout).value();
		static constexpr Flag_t::value_type LR = (Flag::pendingLayout | Flag::pendingRender).value();
		static constexpr Flag_t::value_type  R = (Flag::pendingRender).value();

		Property<Color,             R, pnm::bg_color> bg_color;
		Property<Texture2D_view,   LR, pnm::bg_image> bg_image;
		Property<ShaderImage_view,  R, pnm::bg_shader> bg_shader;

		template <typename T>
		void access(T& ctx) {
			ctx(bg_color, "Background color");
			ctx(bg_image, "Background image");
			ctx(bg_shader, "Background shader");
		}
	};

private:
	libv::glr::Mesh mesh{libv::gl::Primitive::TriangleStrip, libv::gl::BufferUsage::StaticDraw};
public:
	libv::ui::PropertySet<PS> property;

public:
	Stretch();
	Stretch(std::string name);
	Stretch(UnnamedTag_t, const std::string_view type);
	~Stretch();

private:
	virtual void doStyle(ContextStyle& ctx) override;
	virtual void doLayout1(const ContextLayout1& environment) override;
	virtual void doRender(ContextRender& context) override;
};
// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
