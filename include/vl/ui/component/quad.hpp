// File: Quad.hpp, Created on 2014. április 11. 23:31, Author: Vader

#pragma once

//pro
#include "vl/ui/component/component.hpp"
#include "vl/ui/render/default_renderer.hpp"

namespace vl {
namespace ui {

struct Quad : public Component {
	float id = 0;
	float blue = 0;
	bool diretion = false;
	Quad() {
		static float nid = 0;
		nid += 0.25f;
		id = nid;
	}
	virtual void build() override { }
	virtual void update() override {
		if (blue >= 1 || blue <= 0)
			diretion = !diretion;
		blue += diretion ? 0.01f : -0.01f;
	}
	virtual void render() override {
		DefaultRenderer::render(this);
	}
};

} //namespace ui
} //namespace vl
