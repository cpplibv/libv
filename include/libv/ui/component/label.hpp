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
	template <typename T>
	static void access_properties(T& ctx);
//	static ComponentPropertyDescription description;

	struct PS {
		PropertyFFR<Color> font_color;
		PropertyFFR<ShaderFont_view> font_shader;

		PropertyFFL<> align_horizontal;
		PropertyFFL<> font;
		PropertyFFL<> font_size;
	} property;

private:
	String2D text_;

public:
	Label();
	Label(std::string name);
	Label(UnnamedTag_t, const std::string_view type);
	~Label();

public:
	void align_horizontal(AlignHorizontal value, PropertyDriver driver = PropertyDriver::manual);
	AlignHorizontal align_horizontal() const noexcept;

	void font(Font2D_view value, PropertyDriver driver = PropertyDriver::manual);
	const Font2D_view& font() const noexcept;

	void font_size(FontSize value, PropertyDriver driver = PropertyDriver::manual);
	FontSize font_size() const noexcept;

	void text(std::string value);
	const std::string& text() const noexcept;

private:
	virtual void doStyle(ContextStyle& ctx) override;
	virtual void doLayout1(const ContextLayout1& environment) override;
	virtual void doLayout2(const ContextLayout2& environment) override;
	virtual void doRender(ContextRender& context) override;
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
