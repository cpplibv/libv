// File: image.hpp - Created on 2017.10.22. 10:33 - Author: Vader

#pragma once

// libv
#include <libv/glr/mesh.hpp>
#include <libv/math/vec.hpp>
// std
#include <string>
// pro
#include <libv/ui/component_base.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class ContextLayoutPass1;
class ContextRender;
class ContextUI;
class ShaderImage;
class Texture2D;

struct Image : ComponentBase {
private:
	libv::glr::Mesh mesh{libv::gl::Primitive::Triangles, libv::gl::BufferUsage::StaticDraw};
	std::shared_ptr<ShaderImage> shader;
	std::shared_ptr<Texture2D> image_;

	libv::vec4f color_ = {1.f, 1.f, 1.f, 1.f}; // <<< P4: Into properity color_ you go

public:
	Image();

public:
	void image(std::shared_ptr<Texture2D> image_);

public:
	vec4f& color() {
		return color_;
	}
	const vec4f& color() const {
		return color_;
	}
	void color(const vec4f& color_) {
		this->color_ = color_;
	}
	void color(float r, float g, float b, float a = 1.f) {
		color({r, g, b, a});
	}

private:
	virtual void doAttach(ContextUI& context) override;
	virtual void doLayoutPass1(const ContextLayoutPass1& environment) override;
	virtual void doRender(ContextRender& context) override;
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
