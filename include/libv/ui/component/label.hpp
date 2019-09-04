// File: label.hpp - Created on 2017.10.22. 07:12 - Author: Vader

#pragma once

// libv
#include <libv/meta/reflection_access.hpp>
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
		PropertyAlignHorizontal align;
		PropertyFont font;
		PropertyFontColor font_color;
		PropertyFontSize font_size;
		PropertyShaderFont font_shader;

		LIBV_REFLECTION_ACCESS(align);
		LIBV_REFLECTION_ACCESS(font);
		LIBV_REFLECTION_ACCESS(font_color);
		LIBV_REFLECTION_ACCESS(font_shader);
		LIBV_REFLECTION_ACCESS(font_size);
	};

private:
	String2D string;
public:
	libv::ui::PropertySet<PS> properties;

public:
	Label();
	Label(std::string name);
	Label(UnnamedTag, const std::string_view type);
	~Label();

public:
	void setText(std::string string_);
	const std::string& getText() const;

private:
	virtual void doStyle() override;
	virtual void doRender(ContextRender& context) override;
	virtual void doLayout1(const ContextLayout1& environment) override;
	virtual void doLayout2(const ContextLayout2& environment) override;
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
