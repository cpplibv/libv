// File: panel.cpp, Created on 2015. április 8. 14:32, Author: Vader

// hpp
#include <libv/ui/component/label.hpp>
// ext
#include <glm/gtx/transform.hpp>
// pro
#include <libv/gl/log.hpp>
#include <libv/ui/layout.hpp>
#include <libv/ui/log.hpp>
#include <libv/ui/properties_eval.hpp>

namespace libv {
namespace ui {

void Label::setText(const std::string& str) {
	LIBV_LOG_UI_COMPONENT_TRACE("Set label text to [%s]", str);
	invalidate();
	text.setText(str);
}

LayoutInfo Label::doLayout(const LayoutInfo& parentLayoutInfo) {
	auto size = vec_static_cast<int>(evalLayoutSize(parentLayoutInfo, *this).xy());
	text.setSize(size);
	text.build(getContext());
	auto result = maxByDimensions(size, text.getLayoutedSize());
	return vec3(result, 0);
}

void Label::doBuild(Context& context) {
	text.build(context);
}

void Label::doDestroy(Context& context) {
	text.destroy(context);
}

void Label::doRender(Context& context) {
	text.render(context);
}

} //namespace ui
} //namespace libv
