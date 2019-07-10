// File:   property.cpp Author: Vader Created on 24 July 2019, 16:57

// hpp
#include <libv/ui/property.hpp>
// pro
#include <libv/ui/context_ui.hpp>
#include <libv/ui/shader/shader_font.hpp>
#include <libv/ui/shader/shader_image.hpp>
#include <libv/ui/shader/shader_quad.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

PropertyImage::value_type PropertyImage::fallback(ContextUI& context) {
	return context.getFallbackTexture2D();
}

PropertyFont::value_type PropertyFont::fallback(ContextUI& context) {
	return context.font("consola.ttf");
}

PropertyShaderFont::value_type PropertyShaderFont::fallback(ContextUI& context) {
	return context.template shader<ShaderFont>();
}

PropertyShaderImage::value_type PropertyShaderImage::fallback(ContextUI& context) {
	return context.template shader<ShaderImage>();
}

PropertyShaderQuad::value_type PropertyShaderQuad::fallback(ContextUI& context) {
	return context.template shader<ShaderQuad>();
}

//Flag_t PropertySize::invalidate(const value_type& value) {
//	if (value.value.x.content || value.value.y.content || value.value.z.content)
//		return Flag::pendingLayout;
//	else
//		return Flag::pendingLayoutSelf;
//}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
