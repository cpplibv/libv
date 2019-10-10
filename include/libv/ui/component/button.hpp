// File: label.hpp - Created on 2017.10.22. 07:12 - Author: Vader

#pragma once

// libv
#include <libv/glr/mesh.hpp>
// std
#include <functional>
#include <string>
#include <string_view>
// pro
#include <libv/ui/base_component.hpp>
#include <libv/ui/event/mouse_watcher.hpp>
#include <libv/ui/property.hpp>
#include <libv/ui/string_2D.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class Button : public BaseComponent {
private:
	template <typename T>
	static void access_properties(T& ctx);

	struct Properties {
		PropertyR<Color> bg_color;
		PropertyL<Texture2D_view> bg_image;
		PropertyR<ShaderImage_view> bg_shader;

		PropertyR<Color> font_color;
		PropertyR<ShaderFont_view> font_shader;

		PropertyL<> align_horizontal;
		PropertyL<> font;
		PropertyL<> font_size;
	} property;

private:
	libv::glr::Mesh bg_mesh{libv::gl::Primitive::Triangles, libv::gl::BufferUsage::StaticDraw};
	String2D text_;
	MouseWatcher mouseWatcher;

public:
	Button();
	Button(std::string name);
	Button(UnnamedTag_t, const std::string_view type);
	~Button();

public:
	void align_horizontal(AlignHorizontal value, PropertyDriver driver = PropertyDriver::manual);
	AlignHorizontal align_horizontal() const noexcept;

	void font(Font2D_view value, PropertyDriver driver = PropertyDriver::manual);
	const Font2D_view& font() const noexcept;

	void font_size(FontSize value, PropertyDriver driver = PropertyDriver::manual);
	FontSize font_size() const noexcept;

	void text(std::string value);
	const std::string& text() const noexcept;

public:
	// TODO P1: libv.ui Think about a signal-slot pattern very hard
	void setCallback(std::function<void(const EventMouse&)> callback);

private:
	virtual void onFocus(const EventFocus& event) override;

private:
	virtual void doAttach() override;
	virtual void doDetach() override;
	virtual void doStyle(ContextStyle& ctx) override;
	virtual void doLayout1(const ContextLayout1& environment) override;
	virtual void doLayout2(const ContextLayout2& environment) override;
	virtual void doRender(ContextRender& context) override;
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
