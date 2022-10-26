// Project: libv.ui, File: src/libv/ui/component/input_field.hpp

#pragma once

// pro
#include <libv/ui/component_system/component_api.hpp>
#include <libv/ui/property/align.hpp>
#include <libv/ui/property/background.hpp>
#include <libv/ui/property/color.hpp>
#include <libv/ui/property/font_2D.hpp>
#include <libv/ui/property/font_size.hpp>


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
//		int32_t amount;
//		Change change;
//		// Change type: insert/push_back/remove/pop_back | Kind of important, could be used for optimization
};

struct EventCaret : BaseEvent {
	// int32_t previous_caret_position;
};

struct EventSelect : BaseEvent {
};

template <typename ComponentT>
struct EventHostEditable : EventHostSubmittable<ComponentT> {
	BasicEventProxy<ComponentT, EventCaret> caret{this->owner};
	BasicEventProxy<ComponentT, EventChange> change{this->owner};
	BasicEventProxy<ComponentT, EventSelect> select{this->owner};
};

// -------------------------------------------------------------------------------------------------

class InputField : public ComponentAPI<Component, InputField, class CoreInputField, EventHostEditable> {
public:
	using ComponentAPI::ComponentAPI;
	static constexpr std::string_view component_type = "input-field";
	[[nodiscard]] static core_ptr create_core(std::string name);
	[[nodiscard]] static bool castable(core_ptr) noexcept;

public:
	void background(Background value);
	[[nodiscard]] const Background& background() const noexcept;

public:
	void align_horizontal(AlignHorizontal value);
	[[nodiscard]] AlignHorizontal align_horizontal() const noexcept;

	void align_vertical(AlignVertical value);
	[[nodiscard]] AlignVertical align_vertical() const noexcept;

	void font(Font2D_view value);
	[[nodiscard]] const Font2D_view& font() const noexcept;

	void font_size(FontSize value);
	[[nodiscard]] FontSize font_size() const noexcept;

	void font_color(Color value);
	[[nodiscard]] const Color& font_color() const noexcept;

public:
	void text(std::string value);
	[[nodiscard]] const std::string& text() const noexcept;

	/// 0 = Before the first character, n = Before the nth character, length() = After the last character (which is also a perfectly valid position)
	void caret(uint32_t value);
	/// 0 = Before the first character, n = Before the nth character, length() = After the last character (which is also a perfectly valid position)
	[[nodiscard]] uint32_t caret() const noexcept;

	void focus_select_policy(FocusSelectPolicy value);
	[[nodiscard]] FocusSelectPolicy focus_select_policy() const noexcept;
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
