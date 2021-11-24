// Project: libv.ui, File: src/libv/ui/context/context_mouse.hpp

#pragma once

// libv
#include <libv/input/input.hpp>
#include <libv/math/vec.hpp>
#include <libv/utility/function_ref.hpp>
// std
#include <memory>
// pro
#include <libv/ui/event/mouse_order.hpp>


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

	void enable(CoreComponent& component, bool value);

	void update(CoreComponent& component, libv::vec3f position, libv::vec3f size, MouseOrder order);
	void update_region(CoreComponent& component, libv::vec2f remap_offset);

	void unsubscribe(CoreComponent& component);
	void unsubscribe_region(CoreComponent& component);

public:
	void acquire(CoreComponent& component);
	void release(CoreComponent& component);

public:
//	struct ComponentMouseInfo {
//		Component component;
//		libv::vec2f cornerBL;
//		libv::vec2f cornerTR;
//		MouseOrder order;
//	};
//
//	void foreach_mouse_over(libv::function_ref<void(const ComponentMouseInfo&)> func);

public:
	void debug();

public:
	libv::vec2f get_global_position(const CoreComponent& component);

public:
	void event_enter();
	void event_leave();
	void event_button(libv::input::MouseButton mouse, libv::input::Action action);
	void event_position(libv::vec2f position);
	void event_scroll(libv::vec2f movement);

	/// Event update is necessary to support multiple 'subscribe' and 'update' changes atomically
	void event_update_layout();
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
