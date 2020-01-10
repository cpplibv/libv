// File: label.hpp - Created on 2017.10.22. 07:12 - Author: Vader

#pragma once

// libv
#include <libv/glr/mesh.hpp>
#include <libv/input/input.hpp>
// std
#include <functional>
#include <string>
#include <string_view>
// pro
#include <libv/ui/base_component.hpp>
#include <libv/ui/event/event_mouse.hpp>
#include <libv/ui/property.hpp>
#include <libv/ui/string_2D.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class Button : public BaseComponent {
public:
	struct EMouseButton : libv::ui::EventMouseButton {
		Button& component;
	};

	struct EMouseMovement : libv::ui::EventMouseMovement {
		Button& component;
	};

	struct EMouseScroll : libv::ui::EventMouseScroll {
		Button& component;
	};

	struct ESubmit {
		Button& component;
	};

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

public:
	explicit Button(BaseComponent& parent);
	Button(BaseComponent& parent, std::string name);
	Button(BaseComponent& parent, GenerateName_t, const std::string_view type);
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
	template <typename F>
	inline void event_mouseButton(libv::observer_ptr<BaseComponent> slot, F&& func) {
		connect<EMouseButton>(slot, std::forward<F>(func));
	}

	template <typename F>
	inline void event_mouseMovement(libv::observer_ptr<BaseComponent> slot, F&& func) {
		connect<EMouseMovement>(slot, std::forward<F>(func));
	}

	template <typename F>
	inline void event_mouseScroll(libv::observer_ptr<BaseComponent> slot, F&& func) {
		connect<EMouseScroll>(slot, std::forward<F>(func));
	}

	template <typename F>
	inline void event_submit(libv::observer_ptr<BaseComponent> slot, F&& func) {
		connect<ESubmit>(slot, std::forward<F>(func));
	}

private:
	virtual void onFocus(const EventFocus& event) override;
	virtual void onMouseButton(const EventMouseButton& event) override;
	virtual void onMouseMovement(const EventMouseMovement& event) override;
	virtual void onMouseScroll(const EventMouseScroll& event) override;

private:
	virtual void doAttach() override;
	virtual void doStyle(ContextStyle& ctx) override;
	virtual void doLayout1(const ContextLayout1& environment) override;
	virtual void doLayout2(const ContextLayout2& environment) override;
	virtual void doRender(ContextRender& context) override;
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
