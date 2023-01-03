// Project: libv.ui, File: src/libv/ui/context/context_ui.cpp

// hpp
#include <libv/ui/context/context_style.hpp>
// libv
#include <libv/utility/hash_string.hpp>
// std
#include <unordered_map>
// pro
#include <libv/ui/log.hpp>
#include <libv/ui/style/style.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class ImplContextStyle {
public:
	// TODO P5: style unordered_map could be a unordered_set, (generalize dereference hasher)
	std::unordered_map<std::string, libv::intrusive2_ptr<Style>, libv::hash_string, std::equal_to<>> styles;
};

// -------------------------------------------------------------------------------------------------

ContextStyle::ContextStyle() :
	self(std::make_unique<ImplContextStyle>()) {
}

ContextStyle::~ContextStyle() {
	// For the sake of forward declared ptr
}

// -------------------------------------------------------------------------------------------------

bool ContextStyle::isAnyStyleDirty() const noexcept {
	for (const auto& [_, style_] : self->styles)
		if (style_->isDirty())
			return true;

	return false;
}

void ContextStyle::clearEveryStyleDirty() noexcept {
	for (const auto& [_, style_] : self->styles)
		style_->clearDirty();
}

void ContextStyle::foreach_style(libv::function_ref<void(std::string_view name, Style& style)> func) {
	for (const auto& [name, style_] : self->styles)
		func(name, *style_);
}

// -------------------------------------------------------------------------------------------------

libv::intrusive2_ptr<Style> ContextStyle::style(const std::string_view style_name) {
	const auto it = self->styles.find(style_name);
	if (it != self->styles.end())
		return it->second;

	const auto result = self->styles.emplace(style_name, libv::make_intrusive2_ptr<Style>(std::string(style_name))).first->second;
	const auto lastDot = style_name.rfind('.');
	const auto lastGrt = style_name.rfind('>');

	if (lastDot != style_name.npos && (lastGrt == style_name.npos || lastDot > lastGrt))
		// Auto inherit based on dot naming hierarchy
		result->inherit(style(style_name.substr(0, lastDot)));

	log_ui.trace("Created style {}", style_name);

	return result;
}

libv::intrusive2_ptr<Style> ContextStyle::operator()(const std::string_view style_name) {
	return style(style_name);
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
