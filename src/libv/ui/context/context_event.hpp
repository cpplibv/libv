// Project: libv.ui, File: src/libv/ui/context/context_event.hpp

#pragma once

// libv
#include <libv/utility/type_uid.hpp>
// std
#include <functional>
#include <memory>


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
class ContextEvent {
public:
	using ptr = class CoreComponent*;

private:
	std::unique_ptr<class ImplContextEvent> self;

public:
	ContextEvent();
	~ContextEvent();

public:
	void connect(ptr signal, ptr slot, libv::type_uid event_type, bool front, std::function<void(void*, const void*)>&& func);
	void connect_global(ptr slot, libv::type_uid event_type, bool front, std::function<void(void*, const void*)>&& func);

	void disconnect_signal(ptr signal);
	void disconnect_slot(ptr slot);

	void disconnect(ptr signal, ptr slot); /// Exposition only, Implement on demand
	void disconnect(ptr signal, ptr slot, libv::type_uid event_type); /// Exposition only, Implement on demand
	template <typename Event>
	void disconnect(ptr signal, ptr slot); /// Exposition only, Implement on demand

	void fire(ptr signal, libv::type_uid event_type, const void* event_ptr);
	void fire_global(libv::type_uid event_type, const void* event_ptr);
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
