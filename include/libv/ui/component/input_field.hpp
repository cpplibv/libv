// File: label.hpp - Created on 2017.10.22. 07:12 - Author: Vader

#pragma once

// libv
#include <libv/glr/mesh.hpp>
// std
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
	template <typename T>
	static void access_properties(T& ctx);
//	static ComponentPropertyDescription description;

	struct PS {
		PropertyFFR<Color> bg_color;
		PropertyFFL<Texture2D_view> bg_image;
		PropertyFFR<ShaderImage_view> bg_shader;

		PropertyFFR<Color> cursor_color;
		PropertyFFR<ShaderQuad_view> cursor_shader;

		PropertyFFR<Color> font_color;
		PropertyFFR<ShaderFont_view> font_shader;

		PropertyFFL<> align_horizontal;
		PropertyFFL<> font;
		PropertyFFL<> font_size;
	} property;

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
	virtual bool onChar(const libv::input::EventChar& event) override;
	virtual bool onKey(const libv::input::EventKey& event) override;
	virtual void onFocus(const EventFocus& event) override;
	virtual bool onMouse(const EventMouse& event) override;

//private:
//	virtual void doProperty(const EventFocus& event) override;

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
