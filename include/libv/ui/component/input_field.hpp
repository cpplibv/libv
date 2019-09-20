// File: label.hpp - Created on 2017.10.22. 07:12 - Author: Vader

#pragma once

// libv
#include <libv/glr/mesh.hpp>
#include <libv/meta/reflection_access.hpp>
// std
#include <functional>
#include <string>
#include <string_view>
// pro
#include <libv/ui/base_component.hpp>
#include <libv/ui/chrono.hpp>
#include <libv/ui/property.hpp>
#include <libv/ui/property_set.hpp>
#include <libv/ui/string_2D.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class InputField : public BaseComponent {
private:
	struct PS {
		PropertyColor color;
		PropertyImage image;
		PropertyShaderImage image_shader;

		PropertyAlignHorizontal align;
		PropertyFont font;
		PropertyFontColor font_color;
		PropertyFontSize font_size;
		PropertyShaderFont font_shader;

//		PropertyColor cursor_color;
		PropertyShaderQuad cursor_shader;

		LIBV_REFLECTION_ACCESS(color);
		LIBV_REFLECTION_ACCESS(image);
		LIBV_REFLECTION_ACCESS(image_shader);

		LIBV_REFLECTION_ACCESS(align);
		LIBV_REFLECTION_ACCESS(font);
		LIBV_REFLECTION_ACCESS(font_color);
		LIBV_REFLECTION_ACCESS(font_size);
		LIBV_REFLECTION_ACCESS(font_shader);

//		LIBV_REFLECTION_ACCESS(cursor_color);
		LIBV_REFLECTION_ACCESS(cursor_shader);
	};

public:
	libv::ui::PropertySet<PS> property;

private:
	libv::glr::Mesh quad_mesh{libv::gl::Primitive::Triangles, libv::gl::BufferUsage::StaticDraw};
	libv::glr::Mesh cursor_mesh{libv::gl::Primitive::Triangles, libv::gl::BufferUsage::StaticDraw};

private:
	time_point cursorStartTime;
	libv::vec2f cursorPosition;

private:
	String2D text;

public:
	InputField();
	InputField(std::string name);
	InputField(UnnamedTag_t, const std::string_view type);
	~InputField();

public:
	void setText(std::string string_);
	const std::string& getText() const;

private:
	virtual bool onChar(const libv::input::EventChar& event) override;
	virtual bool onKey(const libv::input::EventKey& event) override;
	virtual void onFocus(const EventFocus& event) override;
	virtual bool onMouse(const EventMouse& event) override;

private:
	virtual void doAttach() override;
	virtual void doStyle() override;
	virtual void doLayout1(const ContextLayout1& environment) override;
	virtual void doLayout2(const ContextLayout2& environment) override;
	virtual void doRender(ContextRender& context) override;
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
