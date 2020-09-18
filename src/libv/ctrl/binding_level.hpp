// Project: libv.ctrl, File: src/libv/ctrl/binding_level.hpp, Author: Császár Mátyás [Vader]

#pragma once


namespace libv {
namespace ctrl {

// -------------------------------------------------------------------------------------------------

enum class binding_level : int32_t {
	// NOTE: 0 is used by auto_binding_level hidden value
	primary = 1,   /// Recommended binding to display on HUD or beside menu items
	secondary, /// Recommended bindings to display in tutorials or tip messages
	hidden,    /// Binding that are not recommended to display on UI
};

/// Uses secondary level unless there is no primary level
static constexpr binding_level auto_binding_level = binding_level{0};

// -------------------------------------------------------------------------------------------------

} //namespace ctrl
} //namespace libv
