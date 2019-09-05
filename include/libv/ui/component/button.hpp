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
#include <libv/ui/event/mouse_watcher.hpp>
#include <libv/ui/property.hpp>
#include <libv/ui/property_set.hpp>
#include <libv/ui/string_2D.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

//struct Focus {
//	enum class state_t : int8_t {
//		idle,
//		focused,
//		selected,
//	} state;
//};

class Button : public BaseComponent {
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

		LIBV_REFLECTION_ACCESS(color);
		LIBV_REFLECTION_ACCESS(image);
		LIBV_REFLECTION_ACCESS(image_shader);

		LIBV_REFLECTION_ACCESS(align);
		LIBV_REFLECTION_ACCESS(font);
		LIBV_REFLECTION_ACCESS(font_color);
		LIBV_REFLECTION_ACCESS(font_size);
		LIBV_REFLECTION_ACCESS(font_shader);
	};

private:
	libv::glr::Mesh mesh{libv::gl::Primitive::Triangles, libv::gl::BufferUsage::StaticDraw};
	String2D string;
//	Focus focus;
	MouseWatcher mouseWatcher;

public:
	libv::ui::PropertySet<PS> properties;

public:
	Button();
	Button(std::string name);
	Button(UnnamedTag, const std::string_view type);
	~Button();

public:
	void setText(std::string string_);
	const std::string& getText() const;

	// TODO P1: libv.ui Think about a signal-slot pattern very hard
	void setCallback(std::function<void(const EventMouse&)> callback);

private:
	virtual void doAttach() override;
	virtual void doDetach() override;
	virtual void doStyle() override;
	virtual void doRender(ContextRender& context) override;
	virtual void doLayout1(const ContextLayout1& environment) override;
	virtual void doLayout2(const ContextLayout2& environment) override;
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv