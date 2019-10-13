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
#include <libv/ui/context_event.hpp>
#include <libv/ui/context_ui.hpp>
#include <libv/ui/property.hpp>
#include <libv/ui/string_2D.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class InputField : public BaseComponent {
public:
	enum class FocusSelectPolicy {
		// TODO P5: Implement FocusSelectPolicy
		caretAtEnd = 0,
		caretAtBeginning,
		caretAtHistory,
		selectAll,
	};

public:
	struct EventChange {
		InputField& component;

//		enum class Change : bool {
//			insert = true,
//			remove = false,
//		};
//
//		InputField& component;
//		// Caret position
//		int32_t caret;
//		Change change;
//		// Change type: insert/push_back/remove/pop_back | Important, could be used for optimization
	};
	struct EventSelect {
		InputField& component;
	};
	struct EventSubmit {
		InputField& component;
	};

private:
	template <typename T>
	static void access_properties(T& ctx);
//	static ComponentPropertyDescription description;

	struct Properties {
		PropertyB<FocusSelectPolicy> focus_select_policy;

		PropertyR<Color> bg_color;
		PropertyL<Texture2D_view> bg_image;
		PropertyR<ShaderImage_view> bg_shader;

		PropertyR<Color> caret_color;
		PropertyR<ShaderQuad_view> caret_shader;

		PropertyR<Color> font_color;
		PropertyR<ShaderFont_view> font_shader;

		PropertyL<> align_horizontal;
		PropertyL<> font;
		PropertyL<> font_size;
	} property;

private:
	libv::glr::Mesh bg_mesh{libv::gl::Primitive::Triangles, libv::gl::BufferUsage::StaticDraw};
	libv::glr::Mesh caret_mesh{libv::gl::Primitive::Triangles, libv::gl::BufferUsage::StaticDraw};
	String2D text_;

private:
	time_point caretStartTime;
	libv::vec2f caretPosition;
	uint32_t caret = 0; /// 0 = Before the first character, n = Before the nth character, length() = After the last character

public:
	explicit InputField(BaseComponent& parent);
	InputField(BaseComponent& parent, std::string name);
	InputField(BaseComponent& parent, GenerateName_t, const std::string_view type);
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

public:
	template <typename F>
	inline void event_change(libv::observer_ptr<BaseComponent> slot, F&& func) {
		connect<EventChange>(slot, std::forward<F>(func));
	}

	template <typename F>
	inline void event_select(libv::observer_ptr<BaseComponent> slot, F&& func) {
		connect<EventSelect>(slot, std::forward<F>(func));
	}

	template <typename F>
	inline void event_submit(libv::observer_ptr<BaseComponent> slot, F&& func) {
		connect<EventSubmit>(slot, std::forward<F>(func));
	}

private:
	virtual bool onChar(const libv::input::EventChar& event) override;
	virtual bool onKey(const libv::input::EventKey& event) override;
	virtual void onFocus(const EventFocus& event) override;
	virtual bool onMouseButton(const EventMouseButton& event) override;
	virtual bool onMouseMovement(const EventMouseMovement& event) override;
	virtual bool onMouseScroll(const EventMouseScroll& event) override;

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
