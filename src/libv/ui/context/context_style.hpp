// Project: libv.ui, File: src/libv/ui/context/context_style.hpp

#pragma once

#include <libv/ui/context/context_ui.hpp> // IWYU pragma: export // For convenience
#include <libv/ui/fwd.hpp>
#include <libv/ui/style/style_fwd.hpp>
#include <libv/utility/function_ref.hpp>
#include <libv/utility/memory/intrusive2_ptr.hpp>

#include <memory>
#include <string_view>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class ImplContextStyle;

class ContextStyle {
private:
	std::unique_ptr<ImplContextStyle> self;

public:
	ContextStyle();
	~ContextStyle();

	ContextStyle(const ContextStyle&) = delete;
	ContextStyle(ContextStyle&&) = delete;
	ContextStyle& operator=(const ContextStyle&) = delete;
	ContextStyle& operator=(ContextStyle&&) = delete;

public:
	[[nodiscard]] bool isAnyStyleDirty() const noexcept;
	void clearEveryStyleDirty() noexcept;
	void foreach_style(libv::function_ref<void(std::string_view name, Style& style)> func);

public:
	[[nodiscard]] libv::intrusive2_ptr<Style> style(const std::string_view style_name);
	[[nodiscard]] libv::intrusive2_ptr<Style> operator()(const std::string_view style_name);
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
