// Project: libv.ui, File: src/libv/ui/context/context_event.hpp, Author: Császár Mátyás [Vader]

#pragma once

// std
#include <functional>
#include <memory>
#include <typeindex>


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
	void connect(ptr signal, ptr slot, std::type_index event_type, bool front, bool system, std::function<bool(void*, const void*)>&& func);
	void connect_global(ptr slot, std::type_index event_type, bool front, bool system, std::function<bool(void*, const void*)>&& func);

	void fire(ptr signal, std::type_index event_type, const void* event_ptr);
	void fire_global(std::type_index event_type, const void* event_ptr);

	void disconnect_signal(ptr signal);
	void disconnect_slot(ptr slot);

	void disconnect(ptr signal, ptr slot); /// Exposition only, Implement on demand
	void disconnect(ptr signal, ptr slot, std::type_index event_type); /// Exposition only, Implement on demand
	template <typename Event>
	void disconnect(ptr signal, ptr slot); /// Exposition only, Implement on demand
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
