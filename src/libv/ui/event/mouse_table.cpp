// File:   mouse.cpp Author: Vader Created on 14 August 2019, 14:01

// hpp
#include <libv/ui/event/mouse_table.hpp>
// ext
#include <boost/container/small_vector.hpp>
// libv
#include <libv/algorithm/erase_if_stable.hpp>
#include <libv/algorithm/erase_unstable.hpp>
#include <libv/algorithm/linear_find.hpp>
#include <libv/algorithm/sort.hpp>
#include <libv/math/vec.hpp>
#include <libv/utility/bit_cast.hpp>
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
	struct EntryTarget {
		std::variant<libv::observer_ref<MouseWatcher>, libv::observer_ref<BaseComponent>> target;

	public:
		inline void notify(const EventMouseButton& event) const {
			const auto visitor = libv::overload(
				[&event](const libv::observer_ref<MouseWatcher>& watcher) {
					if (watcher->cb_button)
						watcher->cb_button(event);
				},
				[&event](const libv::observer_ref<BaseComponent>& component) {
					AccessEvent::onMouseButton(*component, event);
				}
			);

			std::visit(visitor, target);
		}

		inline void notify(const EventMouseMovement& event) const {
			const auto visitor = libv::overload(
				[&event](const libv::observer_ref<MouseWatcher>& watcher) {
					if (watcher->cb_movement)
						watcher->cb_movement(event);
				},
				[&event](const libv::observer_ref<BaseComponent>& component) {
					AccessEvent::onMouseMovement(*component, event);
				}
			);

			std::visit(visitor, target);
		}

		inline void notify(const EventMouseScroll& event) const {
			const auto visitor = libv::overload(
				[&event](const libv::observer_ref<MouseWatcher>& watcher) {
					if (watcher->cb_scroll)
						watcher->cb_scroll(event);
				},
				[&event](const libv::observer_ref<BaseComponent>& component) {
					AccessEvent::onMouseScroll(*component, event);
				}
			);

			std::visit(visitor, target);
		}

		inline bool match(MouseWatcher& watcher) const {
			return std::holds_alternative<libv::observer_ref<MouseWatcher>>(target)
					&& std::get<libv::observer_ref<MouseWatcher>>(target) == &watcher;
		}

		inline bool match(BaseComponent& component) const {
			return std::holds_alternative<libv::observer_ref<BaseComponent>>(target)
					&& std::get<libv::observer_ref<BaseComponent>>(target) == &component;
		}
	};

	struct Entry {
		Flag_t interest;
		libv::vec2f cornerBL;
		libv::vec2f cornerTR;
		MouseOrder order;
		EntryTarget target;

		bool over = false;
		bool pendingUpdate = true;

		Entry(Flag_t interest, libv::vec2f cornerBL, libv::vec2f cornerTR, MouseOrder order, libv::observer_ref<MouseWatcher> watcher) :
			interest(interest), cornerBL(cornerBL), cornerTR(cornerTR), order(order), target{watcher} { }

		Entry(Flag_t interest, libv::vec2f cornerBL, libv::vec2f cornerTR, MouseOrder order, libv::observer_ref<BaseComponent> component) :
			interest(interest), cornerBL(cornerBL), cornerTR(cornerTR), order(order), target{component} { }
	};

	std::vector<Entry> entries;
	std::vector<EntryTarget> acquired;

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
		return entry.target.match(component);
	});

	if (it == self->entries.end())
		return log_ui.warn("Attempted to update a not subscribed component: 0x{:016x} {}", libv::bit_cast<size_t>(&component), component.path());

	it->interest = interest;
	it->pendingUpdate = true;
}

void MouseTable::update(BaseComponent& component, libv::vec2f position, libv::vec2f size, MouseOrder order) {
	const auto it = libv::linear_find_if_iterator(self->entries, [&](const ImplMouseTable::Entry& entry) {
		return entry.target.match(component);
	});

	if (it == self->entries.end())
		return log_ui.warn("Attempted to update a not subscribed component: 0x{:016x} {}", libv::bit_cast<size_t>(&component), component.path());

	it->cornerBL = position;
	it->cornerTR = position + size - 1.f;
	it->order = order;
	it->pendingUpdate = true;
}

void MouseTable::update(MouseWatcher& watcher, Flag_t interest) {
	const auto it = libv::linear_find_if_iterator(self->entries, [&](const ImplMouseTable::Entry& entry) {
		return entry.target.match(watcher);
	});

	if (it == self->entries.end())
		return log_ui.warn("Attempted to update a not subscribed watcher: 0x{:016x}", libv::bit_cast<size_t>(&watcher));

	it->interest = interest;
	it->pendingUpdate = true;
}

void MouseTable::update(MouseWatcher& watcher, libv::vec2f position, libv::vec2f size, MouseOrder order) {
	const auto it = libv::linear_find_if_iterator(self->entries, [&](const ImplMouseTable::Entry& entry) {
		return entry.target.match(watcher);
	});

	if (it == self->entries.end())
		return log_ui.warn("Attempted to update a not subscribed watcher: 0x{:016x}", libv::bit_cast<size_t>(&watcher));

	it->cornerBL = position;
	it->cornerTR = position + size - 1.f;
	it->order = order;
	it->pendingUpdate = true;
}

void MouseTable::unsubscribe(BaseComponent& component) {
	const auto it = libv::linear_find_if_iterator(self->entries, [&](const ImplMouseTable::Entry& entry) {
		return entry.target.match(component);
	});

	if (it == self->entries.end())
		return log_ui.warn("Attempted to unsubscribing a not subscribed component: 0x{:016x} {}", libv::bit_cast<size_t>(&component), component.path());

	libv::erase_unstable(self->entries, it);
	release(component);
}

void MouseTable::unsubscribe(MouseWatcher& watcher) {
	const auto it = libv::linear_find_if_iterator(self->entries, [&](const ImplMouseTable::Entry& entry) {
		return entry.target.match(watcher);
	});

	if (it == self->entries.end())
		return log_ui.warn("Attempted to unsubscribing a not subscribed watcher: 0x{:016x}", libv::bit_cast<size_t>(&watcher));

	libv::erase_unstable(self->entries, it);
	release(watcher);
}

// -------------------------------------------------------------------------------------------------

void MouseTable::acquire(BaseComponent& watcher) {
	self->acquired.emplace_back(ImplMouseTable::EntryTarget{watcher});
}

void MouseTable::acquire(MouseWatcher& watcher) {
	self->acquired.emplace_back(ImplMouseTable::EntryTarget{watcher});
}

void MouseTable::release(BaseComponent& watcher) {
	if (self->acquired.empty())
		return;

	libv::erase_if_stable(self->acquired, [&](const auto& i) { return i.match(watcher); });

	if (self->acquired.empty())
		event_position(self->mouse_position);
}

void MouseTable::release(MouseWatcher& watcher) {
	if (self->acquired.empty())
		return;

	libv::erase_if_stable(self->acquired, [&](const auto& i) { return i.match(watcher); });

	if (self->acquired.empty())
		event_position(self->mouse_position);
}

// -------------------------------------------------------------------------------------------------

namespace {

struct Hit {
	libv::observer_ref<const ImplMouseTable::Entry> entry;
};

struct HitEL {
	libv::observer_ref<ImplMouseTable::Entry> entry;
	bool enter;
	bool leave;
};

using Hits = boost::container::small_vector<Hit, 8>;
using HitELs = boost::container::small_vector<HitEL, 8>;

template <typename Hits>
inline void sort_hits(Hits& hits) noexcept {
	libv::sort_unstable(hits, [](const auto& lhs, const auto& rhs) {
		return libv::to_value(lhs.entry->order) > libv::to_value(rhs.entry->order);
	});
}

template <typename Event>
inline void notify_hits(Hits& hits, Event& event) noexcept {
	for (const auto& hit : hits) {
		hit.entry->target.notify(event);
	}
}

template <typename Event>
inline void notify_hits(HitELs& hits, Event& event) noexcept {
	size_t i = 0;

	for (; i < hits.size(); i++) {
		const auto& hit = hits[i];

		event.enter = hit.enter;
		event.leave = hit.leave;

		hit.entry->target.notify(event);

		if (!event.is_pass_through())
			break;
	}

	i++;

	for (; i < hits.size(); i++) {
		const auto& hit = hits[i];

		hit.entry->over = false;

		if (!hit.enter) { // If it's not a new enter for the watcher, leave that watcher
			event.enter = false;
			event.leave = true;

			hit.entry->target.notify(event);
		}
	}
}

} // namespace

// -------------------------------------------------------------------------------------------------

void MouseTable::event_enter() {
	// No-op, the mouse position event that follows the mouse enter event will take care of it
}

void MouseTable::event_leave() {
	EventMouseMovement event;
	event.mouse_movement = {0, 0};
	event.mouse_position = self->mouse_position;
	event.enter = false;
	event.leave = true;

	// Handle acquired mouse
	if (!self->acquired.empty()) {
		event.enter = false;
		event.leave = false;
		for (const auto& entry : self->acquired)
			entry.notify(event);
		return;
	}

	// Gather hits
	Hits hits;

	for (ImplMouseTable::Entry& entry : self->entries) {
		if (!entry.over)
			continue;

		entry.over = false;

		const bool interested = entry.interest.match_any(Flag::watchMouseEnter);
		if (interested)
			hits.push_back({entry});
	}

	sort_hits(hits);
	notify_hits(hits, event);
}

void MouseTable::event_button(libv::input::Mouse mouse, libv::input::Action action) {
	// Define event
	EventMouseButton event;
	event.action = action;
	event.button = mouse;

	// Handle acquired mouse
	if (!self->acquired.empty()) {
		for (const auto& entry : self->acquired)
			entry.notify(event);
		return;
	}

	// Gather hits
	Hits hits;

	for (const ImplMouseTable::Entry& entry : self->entries) {
		if (!entry.over)
			continue;

		const bool interested = entry.interest.match_any(Flag::watchMouseButton);
		if (interested)
			hits.push_back({entry});
	}

	sort_hits(hits);
	notify_hits(hits, event);
}

void MouseTable::event_position(libv::vec2f position_new) {
	// Define event
	const auto movement = position_new - self->mouse_position;
	self->mouse_position = position_new;

	EventMouseMovement event;
	event.mouse_movement = movement;
	event.mouse_position = position_new;

	// Handle acquired mouse
	if (!self->acquired.empty()) {
		event.enter = false;
		event.leave = false;
		for (const auto& entry : self->acquired)
			entry.notify(event);
		return;
	}

	// Gather hits
	HitELs hits;

	for (ImplMouseTable::Entry& entry : self->entries) {
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
			hits.push_back({entry, enter, leave});
	}

	sort_hits(hits);
	notify_hits(hits, event);
}

void MouseTable::event_scroll(libv::vec2f movement) {
	// Define event
	self->scroll_position += movement;
	const auto position = self->scroll_position;

	EventMouseScroll event;
	event.scroll_movement = movement;
	event.scroll_position = position;

	// Handle acquired mouse
	if (!self->acquired.empty()) {
		for (const auto& entry : self->acquired)
			entry.notify(event);
		return;
	}

	// Gather hits
	Hits hits;

	for (const ImplMouseTable::Entry& entry : self->entries) {
		if (!entry.over)
			continue;

		const bool interested = entry.interest.match_any(Flag::watchMouseScroll);
		if (interested)
			hits.push_back({entry});
	}

	sort_hits(hits);
	notify_hits(hits, event);
}

void MouseTable::event_update() {
	EventMouseMovement event;
	event.mouse_movement = {0, 0};
	event.mouse_position = self->mouse_position;

	// Handle acquired mouse
	if (!self->acquired.empty())
		return;

	// Gather hits
	HitELs hits;

	for (ImplMouseTable::Entry& entry : self->entries) {
		if (!entry.pendingUpdate && !entry.over)
			continue;

		const bool over_new = libv::vec::within(self->mouse_position, entry.cornerBL, entry.cornerTR);
		const bool over_old = entry.over;

		entry.pendingUpdate = false;
		entry.over = over_new;

		if (over_old == over_new)
			continue;

		const bool enter = !over_old && over_new;
		const bool leave = over_old && !over_new;

		const bool interested = entry.interest.match_any(Flag::watchMouseEnter);
		if (interested)
			hits.push_back({entry, enter, leave});
	}

	sort_hits(hits);
	notify_hits(hits, event);
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
