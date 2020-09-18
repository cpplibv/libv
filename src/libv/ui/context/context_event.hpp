// Project: libv.ui, File: src/libv/ui/context/context_event.hpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
#include <libv/utility/observer_ptr.hpp>
// std
#include <functional>
#include <memory>
#include <typeindex>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class CoreComponent;
class ImplContextEvent;

class ContextEvent {
public:
	using ptr = libv::observer_ptr<CoreComponent>;

private:
	std::unique_ptr<ImplContextEvent> self;

public:
	ContextEvent();
	~ContextEvent();

public:
	void connect(ptr signal, ptr slot, std::type_index type, std::function<void(void*, const void*)>&& func);
	void fire(ptr signal, std::type_index type, const void* event_ptr);

	void disconnect_signal(ptr signal);
	void disconnect_slot(ptr slot);

	void disconnect(ptr signal, ptr slot); /// Exposition only, Implement on demand
	template <typename Event>
	void disconnect(ptr signal, ptr slot); /// Exposition only, Implement on demand
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
