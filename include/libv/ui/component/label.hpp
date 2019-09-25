// File: label.hpp - Created on 2017.10.22. 07:12 - Author: Vader

#pragma once

// std
#include <string>
#include <string_view>
// pro
#include <libv/ui/base_component.hpp>
#include <libv/ui/property.hpp>
#include <libv/ui/property_set.hpp>
#include <libv/ui/string_2D.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

struct Label : BaseComponent {
private:
	struct PS {
		static constexpr Flag_t::value_type L  = (Flag::pendingLayout).value();
		static constexpr Flag_t::value_type LR = (Flag::pendingLayout | Flag::pendingRender).value();
		static constexpr Flag_t::value_type  R = (Flag::pendingRender).value();

		Property<AlignHorizontal,  L , pnm::align_horizontal> align_horizontal;
		Property<Font2D_view,      LR, pnm::font> font;
		Property<Color,             R, pnm::font_color> font_color;
		Property<FontSize,         LR, pnm::font_size> font_size;
		Property<ShaderFont_view,   R, pnm::font_shader> font_shader;

		template <typename T>
		void access(T& ctx) {
			ctx(align_horizontal, "Horizontal alignment of the text");
			ctx(font, "Font file");
			ctx(font_color, "Font color");
			ctx(font_shader, "Font shader");
			ctx(font_size, "Font size in pixel");
		}
	};

private:
	String2D string;
public:
	libv::ui::PropertySet<PS> property;

public:
	Label();
	Label(std::string name);
	Label(UnnamedTag_t, const std::string_view type);
	~Label();

public:
	void setText(std::string string_);
	const std::string& getText() const;

private:
	virtual void doStyle(ContextStyle& ctx) override;
	virtual void doLayout1(const ContextLayout1& environment) override;
	virtual void doLayout2(const ContextLayout2& environment) override;
	virtual void doRender(ContextRender& context) override;
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
