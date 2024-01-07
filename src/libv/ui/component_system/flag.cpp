// Project: libv.ui, File: src/libv/ui/component_system/flag.cpp

// hpp
#include <libv/ui/component_system/flag.hpp>
// std
#include <sstream>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

std::string to_string(Flag_t flags) {
	std::ostringstream os;

	if (flags.match_any(Flag::render)) os << "render,";
	if (flags.match_any(Flag::layout)) os << "layout,";
	if (flags.match_any(Flag::disabled)) os << "disabled,";
	if (flags.match_any(Flag::disableControlled)) os << "disableControlled,";
	if (flags.match_any(Flag::signal)) os << "signal,";
	if (flags.match_any(Flag::slot)) os << "slot,";
	if (flags.match_any(Flag::focused)) os << "focused,";
//	if (flags.match_any(Flag::____)) os << "____,";
	if (flags.match_any(Flag::pendingAttachChild)) os << "pendingAttachChild,";
	if (flags.match_any(Flag::pendingAttachSelf)) os << "pendingAttachSelf,";
	if (flags.match_any(Flag::pendingStyleChild)) os << "pendingStyleChild,";
	if (flags.match_any(Flag::pendingStyleSelf)) os << "pendingStyleSelf,";
	if (flags.match_any(Flag::pendingLayoutChild)) os << "pendingLayoutChild,";
	if (flags.match_any(Flag::pendingLayoutSelf)) os << "pendingLayoutSelf,";
	if (flags.match_any(Flag::pendingRenderChild)) os << "pendingRenderChild,";
	if (flags.match_any(Flag::pendingRenderSelf)) os << "pendingRenderSelf,";
	if (flags.match_any(Flag::pendingDetachChild)) os << "pendingDetachChild,";
	if (flags.match_any(Flag::pendingDetachSelf)) os << "pendingDetachSelf,";
	if (flags.match_any(Flag::pendingCreate)) os << "pendingCreate,";
//	if (flags.match_any(Flag::awakeSelf)) os << "awakeSelf,";
//	if (flags.match_any(Flag::awakeChild)) os << "awakeChild,";
	if (flags.match_any(Flag::updatedSize)) os << "updatedSize,";
	if (flags.match_any(Flag::updatedPosition)) os << "updatedPosition,";
	if (flags.match_any(Flag::layoutDependsOnContent)) os << "layoutDependsOnContent,";
	if (flags.match_any(Flag::focusableChild)) os << "focusableChild,";
	if (flags.match_any(Flag::focusableSelf)) os << "focusableSelf,";
	if (flags.match_any(Flag::watchChar)) os << "watchChar,";
	if (flags.match_any(Flag::watchKey)) os << "watchKey,";
	// if (flags.match_any(Flag::watchFocus)) os << "watchFocus,";
	if (flags.match_any(Flag::watchMouse)) os << "watchMouse,";
	if (flags.match_any(Flag::floatRegion)) os << "floatRegion,";
//	if (flags.match_any(Flag::focusLinked)) os << "focusLinked,";

	return std::move(os).str();
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
