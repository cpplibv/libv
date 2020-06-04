// File: pcui.hpp Author: Vader Created on 2017. augusztus 24., 22:32

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
