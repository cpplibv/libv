// Project: libv.ui, File: src/libv/ui/context/context_ui_link.hpp, Author: Császár Mátyás [Vader]

#pragma once


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class ContextUI;

[[nodiscard]] ContextUI& current_thread_context() noexcept;
void current_thread_context(ContextUI& context) noexcept;
void clear_current_thread_context() noexcept;

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
