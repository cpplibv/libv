// File:   interest.hpp Author: Vader Created on 15 April 2019, 06:10

#pragma once

// libv
#include <libv/math/vec.hpp>
#include <libv/input/input.hpp>
// std
#include <memory>
// pro
#include <libv/ui/event/mouse_order.hpp>
#include <libv/ui/flag.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class CoreComponent;

class ContextMouse {
	std::unique_ptr<class ImplContextMouse> self;

public:
	ContextMouse();
	~ContextMouse();

public:
	void subscribe(CoreComponent& component);
	void subscribe_region(CoreComponent& component);

	void update(CoreComponent& component, libv::vec3f abs_position, libv::vec3f size, MouseOrder order);

	void unsubscribe(CoreComponent& component);
	void unsubscribe_region(CoreComponent& component);

public:
	void acquire(CoreComponent& component);
	void release(CoreComponent& component);

public:
	void event_enter();
	void event_leave();
	void event_button(libv::input::MouseButton mouse, libv::input::Action action);
	void event_position(libv::vec2f position);
	void event_scroll(libv::vec2f movement);

	/// Event update is necessary to support multiple 'subscribe' and 'update' changes atomically
	void event_update();
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
