// File: Quad.hpp, Created on 2014. április 11. 23:31, Author: Vader

#pragma once

//pro
#include <vl/ui/component/component.hpp>
#include <vl/ui/render/default_renderer.hpp>

namespace vl {
namespace ui {

struct Quad : public Component {
	RenderPrimitiv quad;

	float id = 0;
	float blue = 0;
	bool diretion = false;
	Quad() {
		static float nid = 0;
		nid += 0.25f;
		id = nid;
	}
	virtual void build(Renderer& renderer) override {
		quad = renderer.quad(vec3(getDisplayPosition()), vec3(getDisplaySize()));
	}
	virtual void update() override {
		if (blue >= 1 || blue <= 0)
			diretion = !diretion;
		blue += diretion ? 0.01f : -0.01f;
	}
	virtual void render(Renderer&) override {
		quad.render();
		//		DefaultRenderer::render(this);
	}
};

} //namespace ui
} //namespace vl
