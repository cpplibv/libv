// File:   mouse.cpp Author: Vader Created on 14 August 2019, 14:01

// hpp
#include <libv/ui/context/context_mouse.hpp>
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
//#include <libv/utility/overload.hpp>
// std
#include <variant>
#include <vector>
// pro
#include <libv/ui/core_component.hpp>
#include <libv/ui/event/event_mouse.hpp>
#include <libv/ui/log.hpp>


namespace libv {
namespace ui {

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
//
//	- Question: How does scroll panel and other moving panels are remapping dynamically the MouseRegions?
//		~ event hitting on such dynamic planes should remap and resend the event to either a different region manager or to different dimension
//			Meaning that we remap the event and not the components
//			Meaning we need to find the closest remapping point if we attach a MouseRegion | by walking up the component hierarchy
//	- When the scroll pane moves under the mouse, does it count as mouse movement? | No, but it can count as a leave.
//
//	- What about hotkey with condition that mouse is over ui or canvas object | not sure, but it feels like a "not my problem" for the event system, sounds like hotkey context switches | libv.control contexts are solving this problem
//	- What about drag and drop / drag payload / drag hover payload | it would be nice to have a generic ui drag payload


// -------------------------------------------------------------------------------------------------

struct ImplContextMouse {
	struct EntryTarget {
		libv::observer_ref<CoreComponent> target;

	public:
		inline void notify(const EventMouseButton& event) const {
			AccessEvent::onMouseButton(*target, event);
		}

		inline void notify(const EventMouseMovement& event) const {
			AccessEvent::onMouseMovement(*target, event);
		}

		inline void notify(const EventMouseScroll& event) const {
			AccessEvent::onMouseScroll(*target, event);
		}

		[[nodiscard]] friend constexpr inline bool operator==(const EntryTarget& lhs, const CoreComponent& rhs) noexcept {
			return lhs.target == &rhs;
		}
		[[nodiscard]] friend constexpr inline bool operator==(const CoreComponent& lhs, const EntryTarget& rhs) noexcept {
			return rhs == lhs;
		}
	};

	struct Entry {
		libv::vec2f cornerBL;
		libv::vec2f cornerTR;
		MouseOrder order;
		EntryTarget target;

		bool over = false;
		bool pendingUpdate = true;

//		bool remap_region = false;

		Entry(libv::vec2f cornerBL, libv::vec2f cornerTR, MouseOrder order, libv::observer_ref<CoreComponent> component) :
			cornerBL(cornerBL), cornerTR(cornerTR), order(order), target{component} { }
	};

	std::vector<Entry> entries;
	std::vector<EntryTarget> acquired;

	libv::vec2f mouse_position;
	libv::vec2f scroll_position;
};

// -------------------------------------------------------------------------------------------------

ContextMouse::ContextMouse() :
	self(std::make_unique<ImplContextMouse>()) {
}

ContextMouse::~ContextMouse() = default; // For the sake of forward declared unique_ptr

void ContextMouse::subscribe(CoreComponent& component) {
	self->entries.emplace_back(libv::vec2f{0, 0}, libv::vec2f{-1, -1}, MouseOrder{0}, libv::make_observer_ref(component));
}

void ContextMouse::subscribe_region(CoreComponent& component) {

}

void ContextMouse::update(CoreComponent& component, libv::vec3f abs_position, libv::vec3f size, MouseOrder order) {
	const auto it = libv::linear_find_if_iterator(self->entries, [&](const ImplContextMouse::Entry& entry) {
		return entry.target == component;
	});

	if (it == self->entries.end())
		return log_ui.warn("Attempted to update a not subscribed component: 0x{:016x} {}", libv::bit_cast<size_t>(&component), component.path());

	it->cornerBL = xy(abs_position);
	it->cornerTR = xy(abs_position) + xy(size) - 1.f;
	it->order = order;
	it->pendingUpdate = true;
}

void ContextMouse::unsubscribe(CoreComponent& component) {
	const auto it = libv::linear_find_if_iterator(self->entries, [&](const ImplContextMouse::Entry& entry) {
		return entry.target == component;
	});

	if (it == self->entries.end())
		return log_ui.warn("Attempted to unsubscribing a not subscribed component: 0x{:016x} {}", libv::bit_cast<size_t>(&component), component.path());

	libv::erase_unstable(self->entries, it);
	release(component);
}

void ContextMouse::unsubscribe_region(CoreComponent& component) {

}

// -------------------------------------------------------------------------------------------------

void ContextMouse::acquire(CoreComponent& component) {
	self->acquired.emplace_back(component);
}

void ContextMouse::release(CoreComponent& component) {
	if (self->acquired.empty())
		return;

	libv::erase_if_stable(self->acquired, [&](const auto& i) { return i == component; });

	if (self->acquired.empty())
		event_position(self->mouse_position);
}

// -------------------------------------------------------------------------------------------------

namespace {

struct Hit {
	libv::observer_ref<const ImplContextMouse::Entry> entry;
};

struct HitEL {
	libv::observer_ref<ImplContextMouse::Entry> entry;
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

		if (!hit.enter) { // If it's not a new enter for the component, leave that component
			event.enter = false;
			event.leave = true;

			hit.entry->target.notify(event);
		}
	}
}

} // namespace

// -------------------------------------------------------------------------------------------------

void ContextMouse::event_enter() {
	// No-op, the mouse position event that follows the mouse enter event will take care of it
}

void ContextMouse::event_leave() {
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

	for (ImplContextMouse::Entry& entry : self->entries) {
		if (!entry.over)
			continue;

		entry.over = false;

		hits.push_back({entry});
	}

	sort_hits(hits);
	notify_hits(hits, event);
}

void ContextMouse::event_button(libv::input::MouseButton mouse, libv::input::Action action) {
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

	for (const ImplContextMouse::Entry& entry : self->entries) {
		if (!entry.over)
			continue;

		hits.push_back({entry});
	}

	sort_hits(hits);
	notify_hits(hits, event);
}

void ContextMouse::event_position(libv::vec2f position_new) {
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

	for (ImplContextMouse::Entry& entry : self->entries) {
		const bool over_new = libv::vec::within(position_new, entry.cornerBL, entry.cornerTR);
		const bool over_old = entry.over;

		entry.over = over_new;

		if (!over_old && !over_new)
			continue;

		const bool enter = !over_old && over_new;
		const bool leave = over_old && !over_new;

		hits.push_back({entry, enter, leave});
	}

	sort_hits(hits);
	notify_hits(hits, event);
}

void ContextMouse::event_scroll(libv::vec2f movement) {
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

	for (const ImplContextMouse::Entry& entry : self->entries) {
		if (!entry.over)
			continue;

		hits.push_back({entry});
	}

	sort_hits(hits);
	notify_hits(hits, event);
}

void ContextMouse::event_update() {
	EventMouseMovement event;
	event.mouse_movement = {0, 0};
	event.mouse_position = self->mouse_position;

	// Handle acquired mouse
	if (!self->acquired.empty())
		return;

	// Gather hits
	HitELs hits;

	for (ImplContextMouse::Entry& entry : self->entries) {
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

		hits.push_back({entry, enter, leave});
	}

	sort_hits(hits);
	notify_hits(hits, event);
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv

// =================================================================================================

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
////struct ImplContextMouse {
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
