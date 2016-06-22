// File: panel.cpp, Created on 2015. április 8. 14:32, Author: Vader

// hpp
#include <libv/ui/component/label.hpp>
// ext
#include <glm/gtx/transform.hpp>
// pro
#include <libv/gl/log.hpp>
#include <libv/ui/layout.hpp>
#include <libv/ui/log.hpp>

namespace libv {
namespace ui {

void Label::setText(const std::string& str) {
	LIBV_UI_COMPONENT_TRACE("Set label text to [%s]", str);
	invalidate();
	text.setText(str);
}

LayoutInfo Label::doLayout(const LayoutInfo&) {
	auto property = get(Property::Size);
	text.setSize(property ? ivec2(property->xy()) : ivec2());
	text.build();
	return vec3(text.getLayoutedSize(), 0);
}

void Label::doBuild(Renderer&) {
	text.build();
}

void Label::doDestroy(Renderer&) {
	text.destroy();
}

void Label::doRender(Renderer& gl) {
	text.render(gl);
}

} //namespace ui
} //namespace libv
