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

class BaseComponent;
class ImplMouseTable;
class MouseWatcher;

class MouseTable {
	std::unique_ptr<ImplMouseTable> self;

public:
	MouseTable();
	~MouseTable();

public:
	void subscribe(BaseComponent& watcher, Flag_t interest);
	void subscribe(MouseWatcher& watcher, Flag_t interest);
	void subscribe(MouseWatcher& watcher, Flag_t interest, libv::vec2f position, libv::vec2f size, MouseOrder order);

	void update(BaseComponent& watcher, libv::vec2f position, libv::vec2f size, MouseOrder order);
//	void update(MouseWatcher& watcher, Flag_t interest, libv::vec2f position, libv::vec2f size, MouseOrder order);
//	void update(MouseWatcher& watcher, Flag_t interest, libv::vec2f position, libv::vec2f size);
//	void update(MouseWatcher& watcher, Flag_t interest, MouseOrder order);
//	void update(MouseWatcher& watcher, Flag_t interest);
	void update(MouseWatcher& watcher, libv::vec2f position, libv::vec2f size, MouseOrder order);
//	void update(MouseWatcher& watcher, libv::vec2f position, libv::vec2f size);
//	void update(MouseWatcher& watcher, MouseOrder order);

	void unsubscribe(BaseComponent& watcher);
	void unsubscribe(MouseWatcher& watcher);

	void event_enter();
	void event_leave();
	void event_button(libv::input::Mouse mouse, libv::input::Action action);
	void event_position(libv::vec2f position);
	void event_scroll(libv::vec2f movement);

	/// Event update is necessary to support multiple 'subscribe' and 'update' changes atomically
	void event_update();
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
