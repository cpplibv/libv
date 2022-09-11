// Project: libv.ui, File: src/libv/ui/context/context_event.hpp

#pragma once

// libv
#include <libv/utility/nexus.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

/// Context event is the main UI event hub. Every event signal and slot is connected inside.
///
/// Connection types are:
/// - Normal: Normal connection between two component
/// - Global: Connection between UI Global signal and a normal component
/// - Scope:        Not implemented yet
/// - Named Normal: Not implemented yet
/// - Named Global: Not implemented yet
/// - Named Scope:  Not implemented yet

struct ContextEvent {
	libv::Nexus nexus;
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
