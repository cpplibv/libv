// File: label.hpp - Created on 2017.10.22. 07:12 - Author: Vader

#pragma once

// libv
#include <libv/math/vec.hpp>
// pro
#include <libv/ui/component_base.hpp>
#include <libv/ui/property_set.hpp>
#include <libv/ui/string_2D.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class ContextLayoutPass1;
class ContextLayoutPass2;
class ContextRender;
class ContextUI;
class Font2D;
class ShaderFont;

struct Label : ComponentBase {
public: // <<< P1: public
	String2D string;
//	StyleLabel_view style;
	PropertySetLabel properties;
	std::shared_ptr<ShaderFont> shader;

public:
	Label();

public:
	inline void setText(std::string string_) {
		string.setString(std::move(string_));
		invalidate(Flag::invalidLayout);
	}
	inline const std::string& getText() const {
		return string.getString();
	}

//public:
//	vec4f& color() {
//		return color_;
//	}
//	const vec4f& color() const {
//		return color_;
//	}
//	void color(const vec4f& color_) {
//		this->color_ = color_;
//	}
//	void color(float r, float g, float b, float a = 1.f) {
//		color({r, g, b, a});
//	}

private:
	virtual void doAttach(ContextUI& context) override;
	virtual void doLayoutPass1(const ContextLayoutPass1& environment) override;
	virtual void doLayoutPass2(const ContextLayoutPass2& environment) override;
	virtual void doRender(ContextRender& context) override;
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
