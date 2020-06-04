// File: label.hpp - Created on 2017.10.22. 07:12 - Author: Vader

#pragma once

// pro
#include <libv/ui/component.hpp>
#include <libv/ui/event_host.hpp>
#include <libv/ui/property/align.hpp>
#include <libv/ui/property/color.hpp>
#include <libv/ui/property/font_2D.hpp>
#include <libv/ui/property/font_size.hpp>
#include <libv/ui/property/shader_image.hpp>
#include <libv/ui/property/texture_2D.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

enum class FocusSelectPolicy {
	// TODO P5: Implement FocusSelectPolicy
	caretAtEnd = 0,
//		caretBeforeLastDot,
	caretAtBeginning,
	caretAtHistory,
	selectAll,
};

struct EventChange : BaseEvent {
//		enum class Change : bool {
//			insert = true,
//			remove = false,
//		};
//
//		// Caret position
//		int32_t caret;
//		Change change;
//		// Change type: insert/push_back/remove/pop_back | Kind of important, could be used for optimization
};

struct EventCaret : BaseEvent {
};

struct EventSelect : BaseEvent {
};

template <typename ComponentT>
struct EventHostEditable : EventHostSubmitable<ComponentT> {
	BasicEventProxy<ComponentT, EventCaret> caret;
	BasicEventProxy<ComponentT, EventChange> change;
	BasicEventProxy<ComponentT, EventSelect> select;

	EventHostEditable(ComponentT& core) : EventHostSubmitable<ComponentT>(core),
		caret(core),
		change(core),
		select(core) {}
};

// -------------------------------------------------------------------------------------------------

class InputField : public ComponenetHandler<class CoreInputField, EventHostEditable<InputField>> {
public:
	explicit InputField(std::string name);
	explicit InputField(GenerateName_t = {}, const std::string_view type = "input-field");
	explicit InputField(base_ptr core) noexcept;

public:
	void color(Color value);
	[[nodiscard]] const Color& color() const noexcept;

	void image(Texture2D_view value);
	[[nodiscard]] const Texture2D_view& image() const noexcept;

	void shader(ShaderImage_view value);
	[[nodiscard]] const ShaderImage_view& shader() const noexcept;

public:
	void align_horizontal(AlignHorizontal value);
	[[nodiscard]] AlignHorizontal align_horizontal() const noexcept;

	void align_vertical(AlignVertical value);
	[[nodiscard]] AlignVertical align_vertical() const noexcept;

	void font(Font2D_view value);
	[[nodiscard]] const Font2D_view& font() const noexcept;

	void font_size(FontSize value);
	[[nodiscard]] FontSize font_size() const noexcept;

public:
	void text(std::string value);
	[[nodiscard]] const std::string& text() const noexcept;

	/// 0 = Before the first character, n = Before the nth character, length() = After the last character
	void caret(uint32_t value);
	/// 0 = Before the first character, n = Before the nth character, length() = After the last character
	[[nodiscard]] uint32_t caret() const noexcept;

	void focus_select_policy(FocusSelectPolicy value);
	[[nodiscard]] FocusSelectPolicy focus_select_policy() const noexcept;
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
