// Project: libv.ui, File: src/libv/ui/context/context_ui_link.hpp, Author: Császár Mátyás [Vader]

#pragma once

// fwd
#include <libv/ui/fwd.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

[[nodiscard]] ContextUI& current_thread_context() noexcept;
void current_thread_context(ContextUI& context) noexcept;
void clear_current_thread_context() noexcept;
bool has_current_thread_context() noexcept;

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
