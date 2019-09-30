// File:   mouse.cpp Author: Vader Created on 14 August 2019, 14:01

// hpp
#include <libv/ui/event/mouse_table.hpp>
// ext
#include <boost/container/small_vector.hpp>
// libv
#include <libv/algorithm/erase_unstable.hpp>
#include <libv/algorithm/linear_find.hpp>
#include <libv/algorithm/sort.hpp>
#include <libv/math/vec.hpp>
#include <libv/utility/enum.hpp>
#include <libv/utility/observer_ref.hpp>
#include <libv/utility/overload.hpp>
// std
#include <variant>
#include <vector>
// pro
#include <libv/ui/base_component.hpp>
#include <libv/ui/event/event_mouse.hpp>
#include <libv/ui/event/mouse_watcher.hpp>
#include <libv/ui/log.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

//#include <boost/geometry.hpp>
//#include <boost/geometry/geometries/point.hpp>
//#include <boost/geometry/geometries/box.hpp>
//#include <boost/geometry/index/rtree.hpp>
////#include <boost/geometry/index/predicates.hpp>
////#include <boost/geometry/core/cs.hpp>
//
// https://www.boost.org/doc/libs/1_70_0/libs/geometry/doc/html/geometry/spatial_indexes/introduction.html
//
//struct ImplMouseRegionHandler {
//	Interest_t interest;
//	libv::observer_ref<MouseRegion> region;
////	MouseRegionDimension dimension;
//};
//
////struct ImplMouseTable {
//struct ImplMouseDimension {
//	using point = boost::geometry::model::point<float, 2, boost::geometry::cs::cartesian>;
//	using region = boost::geometry::model::box<point>;
//
////	using value_type = ImplMouseRegionHandler;
//	using value_type = MouseDimension;
//    using key_type = region;
//
//    using key_value = std::pair<key_type, value_type>;
//    using rtree = boost::geometry::index::rtree<key_value, boost::geometry::index::quadratic<16>>;
//
//public:
//	rtree container;
//};

// -------------------------------------------------------------------------------------------------

//Mouse event requirements:
//
//	- 1 callback (for memory footprint reasons, branching on variant is cheaper (ECS can change this))
//	- Even with a hotkey system in place, There is viable usecases for querying keyboard (and other input) states in the event callbacks
//	- Events (including focus, key, char and mouse) has to be broadcaster serialized
//
//	- By default every component should be click through
//	- To prevent click through an absorb/shield/plate is necessary (for ui window or solid ui elements)
//	- There should be an easy and generic way for non interactive components to shield mouse events
//
//	- If two area/region is affected by an event both should be notified
//	- When an event dispatched to multiple handler the order should be deterministic (and logical) | (ehhh, kind of in order, within order it pseudo random)
//
//	- Every MouseRegion shall move with its owner (component) | aka update in doLayout2 | is there a way to automate it? | I think there is! | at least partially with another component based type sub
//	- Every enter event is guaranteed to be paired with a leave event (unless an unsubscription, interest change or disable happens)
//	- Layout should be an atomic step what mouse concerns, this means layout update cannot produce ANY event
//	- When layout changes but it stays above the same object(s) a mouse movement event with (0,0) should be sent
//
//	- Mouse events should move around focus | (not entirely true, but mouse event and focus/selection modules will interact)
//	- Question: How does scroll panel and other moving panels are remapping dynamically the MouseRegions?
//		~ event hitting on such dynamic planes should remap and resend the event to either a different region manager or to different dimension
//			Meaning that we remap the event and not the components
//			Meaning we need to find the closest remapping point if we attach a MouseRegion | by walking up the component hierarchy
//	- When the scroll pane moves under the mouse, does it count as mouse movement? | No, but it can count as a leave.
//	- What about hotkey with condition that mouse is over ui or canvas object | not sure, but it feels like a "not my problem" for the event system, sounds like hotkey context switches
//	- What about drag and drop / drag payload / drag hover payload | it would be nice to have a generic ui drag payload


// -------------------------------------------------------------------------------------------------

struct ImplMouseTable {
	struct Entry {
		Flag_t interest;
		libv::vec2f cornerBL;
		libv::vec2f cornerTR;
		MouseOrder order;
		std::variant<
			libv::observer_ref<MouseWatcher>,
			libv::observer_ref<BaseComponent>
		> target;

		bool over = false;
		bool pendingUpdate = true;

		Entry(Flag_t interest, libv::vec2f cornerBL, libv::vec2f cornerTR, MouseOrder order, libv::observer_ref<MouseWatcher> watcher) :
			interest(interest), cornerBL(cornerBL), cornerTR(cornerTR), order(order), target(watcher) { }

		Entry(Flag_t interest, libv::vec2f cornerBL, libv::vec2f cornerTR, MouseOrder order, libv::observer_ref<BaseComponent> component) :
			interest(interest), cornerBL(cornerBL), cornerTR(cornerTR), order(order), target(component) { }

		inline void notify(const EventMouse& event) const {
			const auto visitor = libv::overload(
				[&event](const libv::observer_ref<MouseWatcher>& watcher) {
					if (watcher->callback)
						watcher->callback(event);
				},
				[&event](const libv::observer_ref<BaseComponent>& component) {
					AccessEvent::onMouse(*component, event);
					// <<< P5: Mouse shield: return value for shielding is discarded
				}
			);

			std::visit(visitor, target);
		}

		inline bool match(MouseWatcher& watcher) const {
			return std::holds_alternative<libv::observer_ref<MouseWatcher>>(target)
					&& std::get<libv::observer_ref<MouseWatcher>>(target) == libv::make_observer_ref(watcher);
		}

		inline bool match(BaseComponent& component) const {
			return std::holds_alternative<libv::observer_ref<BaseComponent>>(target)
					&& std::get<libv::observer_ref<BaseComponent>>(target) == libv::make_observer_ref(component);
		}
	};

	std::vector<Entry> entries;

	libv::vec2f mouse_position;
	libv::vec2f scroll_position;
};

// -------------------------------------------------------------------------------------------------

MouseTable::MouseTable() :
	self(std::make_unique<ImplMouseTable>()) {
}

MouseTable::~MouseTable() { }

void MouseTable::subscribe(BaseComponent& component, Flag_t interest) {
	self->entries.emplace_back(interest, libv::vec2f{0, 0}, libv::vec2f{-1, -1}, MouseOrder{0}, libv::make_observer_ref(component));
}

void MouseTable::subscribe(MouseWatcher& watcher, Flag_t interest) {
	self->entries.emplace_back(interest, libv::vec2f{0, 0}, libv::vec2f{-1, -1}, MouseOrder{0}, libv::make_observer_ref(watcher));
}

void MouseTable::subscribe(MouseWatcher& watcher, Flag_t interest, libv::vec2f position, libv::vec2f size, MouseOrder order) {
	self->entries.emplace_back(interest, position, position + size - 1.f, order, libv::make_observer_ref(watcher));
}

void MouseTable::update(BaseComponent& component, Flag_t interest) {
	const auto it = libv::linear_find_if_iterator(self->entries, [&](const ImplMouseTable::Entry& entry) {
		return entry.match(component);
	});

	if (it == self->entries.end())
		return log_ui.warn("Attempted to update a not subscribed component: 0x{:016x} {}", reinterpret_cast<size_t>(&component), component.path());

	it->interest = interest;
	it->pendingUpdate = true;
}

void MouseTable::update(BaseComponent& component, libv::vec2f position, libv::vec2f size, MouseOrder order) {
	const auto it = libv::linear_find_if_iterator(self->entries, [&](const ImplMouseTable::Entry& entry) {
		return entry.match(component);
	});

	if (it == self->entries.end())
		return log_ui.warn("Attempted to update a not subscribed component: 0x{:016x} {}", reinterpret_cast<size_t>(&component), component.path());

	it->cornerBL = position;
	it->cornerTR = position + size - 1.f;
	it->order = order;
	it->pendingUpdate = true;
}

void MouseTable::update(MouseWatcher& watcher, Flag_t interest) {
	const auto it = libv::linear_find_if_iterator(self->entries, [&](const ImplMouseTable::Entry& entry) {
		return entry.match(watcher);
	});

	if (it == self->entries.end())
		return log_ui.warn("Attempted to update a not subscribed watcher: 0x{:016x}", reinterpret_cast<size_t>(&watcher));

	it->interest = interest;
	it->pendingUpdate = true;
}

void MouseTable::update(MouseWatcher& watcher, libv::vec2f position, libv::vec2f size, MouseOrder order) {
	const auto it = libv::linear_find_if_iterator(self->entries, [&](const ImplMouseTable::Entry& entry) {
		return entry.match(watcher);
	});

	if (it == self->entries.end())
		return log_ui.warn("Attempted to update a not subscribed watcher: 0x{:016x}", reinterpret_cast<size_t>(&watcher));

	it->cornerBL = position;
	it->cornerTR = position + size - 1.f;
	it->order = order;
	it->pendingUpdate = true;
}

void MouseTable::unsubscribe(BaseComponent& component) {
	const auto it = libv::linear_find_if_iterator(self->entries, [&](const ImplMouseTable::Entry& entry) {
		return entry.match(component);
	});

	if (it == self->entries.end())
		return log_ui.warn("Attempted to unsubscribing a not subscribed component: 0x{:016x} {}", reinterpret_cast<size_t>(&component), component.path());

	libv::erase_unstable(self->entries, it);
}

void MouseTable::unsubscribe(MouseWatcher& watcher) {
	const auto it = libv::linear_find_if_iterator(self->entries, [&](const ImplMouseTable::Entry& entry) {
		return entry.match(watcher);
	});

	if (it == self->entries.end())
		return log_ui.warn("Attempted to unsubscribing a not subscribed watcher: 0x{:016x}", reinterpret_cast<size_t>(&watcher));

	libv::erase_unstable(self->entries, it);
}

// -------------------------------------------------------------------------------------------------

void MouseTable::event_enter() {
	// No-op, the mouse position event that follows will take care of it
}

void MouseTable::event_leave() {
	using Entry = ImplMouseTable::Entry;

	EventMouse::Movement event;
	event.mouse_movement = {0, 0};
	event.mouse_position = self->mouse_position;

	// Gather hits
	struct Hit {
		libv::observer_ref<Entry> entry;

		Hit(libv::observer_ref<Entry> entry) : entry(entry) { }
	};
	boost::container::small_vector<Hit, 8> hits;

	for (ImplMouseTable::Entry& entry : self->entries) {
		if (!entry.over)
			continue;

		entry.over = false;

		const bool interested = entry.interest.match_any(Flag::watchMouseEnter);

		if (interested)
			hits.emplace_back(libv::make_observer_ref(entry));
	}

	// Sort hits by order
	libv::sort(hits, [](const Hit& lhs, const Hit& rhs) {
		return libv::to_value(lhs.entry->order) > libv::to_value(rhs.entry->order);
	});

	// Notify hits in order
	for (const Hit& hit : hits) {
		event.enter = false;
		event.leave = true;

		hit.entry->notify(EventMouse{event}); // TODO P5: nicer construction of EventMouse
	}
}

void MouseTable::event_button(libv::input::Mouse mouse, libv::input::Action action) {
	using Entry = ImplMouseTable::Entry;

	// Define event
	EventMouse::Button event;
	event.action = action;
	event.button = mouse;

	// Gather hits
	struct Hit {
		libv::observer_ref<const Entry> entry;
		Hit(libv::observer_ref<const Entry> entry) : entry(entry) { }
	};
	boost::container::small_vector<Hit, 8> hits;

	for (const Entry& entry : self->entries) {
		const auto over = libv::vec::within(self->mouse_position, entry.cornerBL, entry.cornerTR);

		if (!over)
			continue;

		const bool interested = entry.interest.match_any(Flag::watchMouseButton);
		if (interested)
			hits.emplace_back(libv::make_observer_ref(entry));
	}

	// Sort hits by order
	libv::sort(hits, [](const Hit& lhs, const Hit& rhs) {
		return libv::to_value(lhs.entry->order) > libv::to_value(rhs.entry->order);
	});

	// Notify hits in order
	for (const Hit& hit : hits)
		hit.entry->notify(EventMouse{event}); // TODO P5: nicer construction of EventMouse
}

void MouseTable::event_position(libv::vec2f position_new) {
	using Entry = ImplMouseTable::Entry;

	// Define event
	const auto movement = position_new - self->mouse_position;
	self->mouse_position = position_new;

	EventMouse::Movement event;
	event.mouse_movement = movement;
	event.mouse_position = position_new;

	// Gather hits
	struct Hit {
		libv::observer_ref<const Entry> entry;
		bool enter;
		bool leave;

		Hit(libv::observer_ref<const Entry> entry, bool enter, bool leave) :
			entry(entry), enter(enter), leave(leave) { }
	};
	boost::container::small_vector<Hit, 8> hits;

	for (Entry& entry : self->entries) {
		const bool over_new = libv::vec::within(position_new, entry.cornerBL, entry.cornerTR);
		const bool over_old = entry.over;
		entry.over = over_new;

		if (!over_old && !over_new)
			continue;

		const bool enter = !over_old && over_new;
		const bool leave = over_old && !over_new;

		const bool interested =
				entry.interest.match_any(Flag::watchMousePosition) ||
				(over_old != over_new && entry.interest.match_any(Flag::watchMouseEnter));

		if (interested)
			hits.emplace_back(libv::make_observer_ref(entry), enter, leave);
	}

	// Sort hits by order
	libv::sort(hits, [](const Hit& lhs, const Hit& rhs) {
		return libv::to_value(lhs.entry->order) > libv::to_value(rhs.entry->order);
	});

	// Notify hits in order
	for (const Hit& hit : hits) {
		event.enter = hit.enter;
		event.leave = hit.leave;

		hit.entry->notify(EventMouse{event}); // TODO P5: nicer construction of EventMouse
	}
}

void MouseTable::event_scroll(libv::vec2f movement) {
	using Entry = ImplMouseTable::Entry;

	// Define event
	self->scroll_position += movement;
	const auto position = self->scroll_position;

	EventMouse::Scroll event;
	event.scroll_movement = movement;
	event.scroll_position = position;

	// Gather hits
	struct Hit {
		libv::observer_ref<const Entry> entry;
		Hit(libv::observer_ref<const Entry> entry) : entry(entry) { }
	};
	boost::container::small_vector<Hit, 8> hits;

	for (const Entry& entry : self->entries) {
		if (!entry.over)
			continue;

		const bool interested = entry.interest.match_any(Flag::watchMouseScroll);

		if (interested)
			hits.emplace_back(libv::make_observer_ref(entry));
	}

	// Sort hits by order
	libv::sort(hits, [](const Hit& lhs, const Hit& rhs) {
		return libv::to_value(lhs.entry->order) > libv::to_value(rhs.entry->order);
	});

	// Notify hits in order
	for (const Hit& hit : hits)
		hit.entry->notify(EventMouse{event}); // TODO P5: nicer construction of EventMouse
}

void MouseTable::event_update() {
	using Entry = ImplMouseTable::Entry;

	EventMouse::Movement event;
	event.mouse_movement = {0, 0};
	event.mouse_position = self->mouse_position;

	// Gather hits
	struct Hit {
		libv::observer_ref<Entry> entry;
		bool enter;
		bool leave;

		Hit(libv::observer_ref<Entry> entry, bool enter, bool leave) :
			entry(entry), enter(enter), leave(leave) { }
	};
	boost::container::small_vector<Hit, 8> hits;

	for (ImplMouseTable::Entry& entry : self->entries) {
		// <<< P5: Mouse shield: In future when shielding happens this if will have to change
		if (!entry.pendingUpdate)
			continue;

		const auto over_new = libv::vec::within(self->mouse_position, entry.cornerBL, entry.cornerTR);
		const bool over_old = entry.over;

		entry.pendingUpdate = false;
		entry.over = over_new;

		if (!over_old && !over_new)
			continue;

		const bool enter = !over_old && over_new;
		const bool leave = over_old && !over_new;

		const bool interested = over_old != over_new && entry.interest.match_any(Flag::watchMouseEnter);

		if (interested)
			hits.emplace_back(libv::make_observer_ref(entry), enter, leave);
	}

	// Sort hits by order
	libv::sort(hits, [](const Hit& lhs, const Hit& rhs) {
		return libv::to_value(lhs.entry->order) > libv::to_value(rhs.entry->order);
	});

	// Notify hits in order
	for (const Hit& hit : hits) {
		event.enter = hit.enter;
		event.leave = hit.leave;

		hit.entry->notify(EventMouse{event}); // TODO P5: nicer construction of EventMouse
	}
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
