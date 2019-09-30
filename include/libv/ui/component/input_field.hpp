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
public:
	void align_horizontal(AlignHorizontal value);
	AlignHorizontal align_horizontal() const noexcept;

	void font(Font2D_view value);
	const Font2D_view& font() const noexcept;

	void font_size(FontSize value);
	FontSize font_size() const noexcept;

	void text(std::string value);
	const std::string& text() const noexcept;

private:
	struct PS {
		static constexpr Flag_t::value_type L  = (Flag::pendingLayout).value();
		static constexpr Flag_t::value_type LR = (Flag::pendingLayout | Flag::pendingRender).value();
		static constexpr Flag_t::value_type  R = (Flag::pendingRender).value();

		Property<Color,             R, pnm::bg_color> bg_color;
		Property<Texture2D_view,   LR, pnm::bg_image> bg_image;
		Property<ShaderImage_view,  R, pnm::bg_shader> bg_shader;

		Property<Color,             R, pnm::cursor_color> cursor_color;
		Property<ShaderQuad_view,   R, pnm::cursor_shader> cursor_shader;

		Property<Color,             R, pnm::font_color> font_color;
		Property<ShaderFont_view,   R, pnm::font_shader> font_shader;

		PropertySG<InputField, AlignHorizontal, &InputField::align_horizontal, AlignHorizontal, &InputField::align_horizontal, pnm::align_horizontal> align_horizontal;
		PropertySG<InputField, Font2D_view, &InputField::font, const Font2D_view&, &InputField::font, pnm::font> font;
		PropertySG<InputField, FontSize, &InputField::font_size, FontSize, &InputField::font_size, pnm::font_size> font_size;
		PropertySG<InputField, std::string, &InputField::text, const std::string&, &InputField::text, pnm::text> text;

		template <typename T>
		void access(T& ctx) {
			ctx(bg_color, "Background color");
			ctx(bg_image, "Background image");
			ctx(bg_shader, "Background shader");

			ctx(cursor_color, "Cursor color");
			ctx(cursor_shader, "Cursor shader");

			ctx(align_horizontal, "Horizontal alignment of the text");
		//	ctx(align_vertical, "Vertical alignment of the text");
			ctx(font, "Font file");
			ctx(font_color, "Font color");
			ctx(font_shader, "Font shader");
			ctx(font_size, "Font size in pixel");
		}
	};

	libv::ui::PropertySet<PS> property;

private:
	libv::glr::Mesh bg_mesh{libv::gl::Primitive::Triangles, libv::gl::BufferUsage::StaticDraw};
	libv::glr::Mesh cursor_mesh{libv::gl::Primitive::Triangles, libv::gl::BufferUsage::StaticDraw};

private:
	time_point cursorStartTime;
	libv::vec2f cursorPosition;

private:
	String2D text_;

public:
	InputField();
	InputField(std::string name);
	InputField(UnnamedTag_t, const std::string_view type);
	~InputField();

private:
	virtual bool onChar(const libv::input::EventChar& event) override;
	virtual bool onKey(const libv::input::EventKey& event) override;
	virtual void onFocus(const EventFocus& event) override;
	virtual bool onMouse(const EventMouse& event) override;

private:
	virtual void doAttach() override;
	virtual void doStyle(ContextStyle& context) override;
	virtual void doLayout1(const ContextLayout1& environment) override;
	virtual void doLayout2(const ContextLayout2& environment) override;
	virtual void doRender(ContextRender& context) override;
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
