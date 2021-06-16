// Project: libv.ui, File: src/libv/ui/context/context_mouse.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <libv/ui/context/context_mouse.hpp>
// ext
#include <boost/container/small_vector.hpp>
// libv
#include <libv/algo/erase_if_stable.hpp>
#include <libv/algo/erase_unstable.hpp>
#include <libv/algo/sort.hpp>
#include <libv/math/vec.hpp>
#include <libv/utility/to_underlying.hpp>
// std
#include <unordered_map>
#include <vector>
// pro
#include <libv/ui/component/detail/core_component.hpp>
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

namespace {

inline void component_notify(CoreComponent* target, const EventMouseButton& event) {
	AccessEvent::onMouseButton(*target, event);
}

inline void component_notify(CoreComponent* target, const EventMouseMovement& event) {
	AccessEvent::onMouseMovement(*target, event);
}

inline void component_notify(CoreComponent* target, const EventMouseScroll& event) {
	AccessEvent::onMouseScroll(*target, event);
}

[[nodiscard]] inline CoreComponent* component_region(CoreComponent& target) noexcept {
	for (auto it = target.parent(); it != it->parent(); it = it->parent())
		if (it->isFloatRegion())
			return &*it;

	return nullptr;
}

// ---

using MouseKey = CoreComponent*;

class MouseRegionContainer {
public:
	struct Node {
		const MouseKey target;

		libv::vec2f cornerBL = {0, 0};
		libv::vec2f cornerTR = {-1, -1};
		MouseOrder order = MouseOrder{0};

		bool over = false; /// Mouse pointer is over (but might be clipped by region)
		bool inside = false; /// Mouse pointer is over and it is interacting with the node (relates to pass_through)
		bool pendingUpdate = true;

		Node* region_parent = nullptr;
		libv::vec2f region_offset = {0, 0};
		std::vector<Node*> region_nodes;

		explicit inline Node(MouseKey target) noexcept :
			target(target) {}

		[[nodiscard]] constexpr inline libv::vec2f global_offset() const noexcept {
			libv::vec2f result;

			for (auto it = this; it != nullptr; it = it->region_parent)
				result += it->region_offset;

			return result;
		}
	};

private:
	// NOTE: unordered_map's memory address stability is utilized in the nodes
	std::unordered_map<MouseKey, Node> index_and_storage;
	Node* root;

public:
	MouseRegionContainer() :
		root(&index_and_storage.emplace(nullptr, nullptr).first->second) {
	}

public:
	void add_node(MouseKey parent_key, MouseKey new_key) {
		auto* parent_node = find(parent_key);

		if (parent_node == nullptr) {
			log_ui.error("Internal error: {} parent of mouse region {} is missing", parent_key == nullptr ? "<<mouse-root>>" : parent_key->path(), new_key->path());
			assert(false && "Internal error: expected parent of mouse region is missing");
			return;
		}

		const auto emplace_result = index_and_storage.emplace(new_key, new_key);

		if (!emplace_result.second) {
			log_ui.warn("Mouse region {} is already present", parent_key == nullptr ? "<<mouse-root>>" : parent_key->path(), new_key->path());
			return;
		}

		auto& new_node = emplace_result.first->second;

		new_node.region_parent = parent_node;
		parent_node->region_nodes.emplace_back(&new_node);
	}

	void add_region(MouseKey parent_key, MouseKey new_key) {
		add_node(parent_key, new_key);
	}

	Node* find(MouseKey key) noexcept {
		const auto it = index_and_storage.find(key);

		if (it != index_and_storage.end())
			return &it->second;

		return nullptr;
	}

	bool remove(MouseKey key) noexcept {
		const auto it = index_and_storage.find(key);

		if (it == index_and_storage.end())
			return false;

		auto& node = it->second;

		if (!node.region_nodes.empty()) {
			log_ui.error("Internal error: Removing {} parent with {} children", key == nullptr ? "<<mouse-root>>" : key->path(), node.region_nodes.size());
			assert(false && "Internal error: Removing parent with children");
			return true; // The remove still can be considered successful for the caller
		}

		libv::erase_unstable(node.region_parent->region_nodes, &node);
		index_and_storage.erase(it);
		return true;
	}

public:
//	void aux_debug(Node& node, int depth = 0) noexcept {
//		if (depth != 0) {
//			if (node.region_nodes.empty())
//				log_ui.info("{:<60} [{:>9}-{:>9}]({})", std::string((depth - 1) * 4, ' ') +
//						node.target->path(), node.global_offset() + node.cornerBL, node.global_offset() + node.cornerTR, node.over ? "X" : " ");
//			else
//				log_ui.info("{:<60} [{:>9}-{:>9}]({}) -> {}", std::string((depth - 1) * 4, ' ') +
//						node.target->path(), node.global_offset() + node.cornerBL, node.global_offset() + node.cornerTR, node.over ? "X" : " ", node.region_offset);
//		}
//
//		for (auto& child_node : node.region_nodes)
//			aux_debug(*child_node, depth + 1);
//	}
//
//	void debug() noexcept {
//		aux_debug(*root);
//	}

	template <typename F>
	void aux_foreach_tree(Node& node, const F& f, libv::vec2f offset) noexcept {
		const auto visit_childs = f(node, offset);

		if (visit_childs) {
			const auto child_offset = offset + node.region_offset;
			for (auto* child_node : node.region_nodes)
				aux_foreach_tree(*child_node, f, child_offset);
		}
	}

public:
	template <typename F>
	void foreach_tree(const F& f) noexcept {
		for (auto* node : root->region_nodes)
			aux_foreach_tree(*node, f, {});
	}
};

} // namespace

// -------------------------------------------------------------------------------------------------

class ImplContextMouse {
public:
	MouseRegionContainer container;
	std::vector<MouseRegionContainer::Node*> acquired;

public:
	libv::vec2f mouse_position;
	libv::vec2f scroll_position;
};

// -------------------------------------------------------------------------------------------------

ContextMouse::ContextMouse() :
	self(std::make_unique<ImplContextMouse>()) {
}

ContextMouse::~ContextMouse() = default; // For the sake of forward declared unique_ptr

void ContextMouse::subscribe(CoreComponent& component) {
	self->container.add_node(component_region(component), &component);
}

void ContextMouse::subscribe_region(CoreComponent& component) {
	self->container.add_region(component_region(component), &component);
}

void ContextMouse::update(CoreComponent& component, libv::vec3f abs_position, libv::vec3f size, MouseOrder order) {
	auto it = self->container.find(&component);

	if (it == nullptr)
		return log_ui.warn("Attempted to update a not subscribed component: {} {}", static_cast<void*>(&component), component.path());

	it->cornerBL = xy(abs_position);
	it->cornerTR = xy(abs_position) + xy(size) - 1.f;
	it->order = order;
	it->pendingUpdate = true;
}

void ContextMouse::update_region(CoreComponent& component, libv::vec2f remap_offset) {
	auto it = self->container.find(&component);

	if (it == nullptr)
		return log_ui.warn("Attempted to update a not subscribed region: {} {}", static_cast<void*>(&component), component.path());

	it->region_offset = remap_offset;
}

void ContextMouse::unsubscribe(CoreComponent& component) {
	auto success = self->container.remove(&component);

	if (!success)
		return log_ui.warn("Attempted to unsubscribing a not subscribed component: {} {}", static_cast<void*>(&component), component.path());

	release(component);
}

void ContextMouse::unsubscribe_region(CoreComponent& component) {
	auto success = self->container.remove(&component);

	if (!success)
		return log_ui.warn("Attempted to unsubscribing a not subscribed region: {} {}", static_cast<void*>(&component), component.path());

	release(component);
}

// -------------------------------------------------------------------------------------------------

void ContextMouse::acquire(CoreComponent& component) {
	auto* node = self->container.find(&component);

	if (node == nullptr)
		return log_ui.warn("Attempted to acquire a not subscribed component: {} {}", static_cast<void*>(&component), component.path());

	self->acquired.emplace_back(node);
}

void ContextMouse::release(CoreComponent& component) {
	if (self->acquired.empty())
		return;

	libv::erase_if_stable(self->acquired, [&](const auto& i) { return i->target == &component; });

	if (self->acquired.empty())
		event_position(self->mouse_position);
}

// -------------------------------------------------------------------------------------------------

namespace {

struct Hit {
	MouseRegionContainer::Node* node;
	libv::vec2f local_position;
};

struct HitEL {
	MouseRegionContainer::Node* node;
	libv::vec2f local_position;
	bool enter;
	bool leave;
};

using Hits = boost::container::small_vector<Hit, 16>;
using HitELs = boost::container::small_vector<HitEL, 16>;

template <typename Hits>
inline void sort_hits(Hits& hits) noexcept {
	libv::sort_unstable(hits, [](const auto& lhs, const auto& rhs) {
		return libv::to_value(lhs.node->order) > libv::to_value(rhs.node->order);
	});
}

template <typename Event>
inline void notify_hits(Hits& hits, Event& event) noexcept {
	for (const auto& hit : hits) {
		if (!hit.node->inside)
			continue;

		event.local_position = hit.local_position;
		component_notify(hit.node->target, event);
	}
}

template <typename Event>
inline void notify_hits(HitELs& hits, Event& event) noexcept {
	size_t i = 0;

	for (; i < hits.size(); i++) {
		const auto& hit = hits[i];

		if (!hit.node->inside && !hit.enter)
			continue;

		hit.node->inside = !hit.leave;

		event.local_position = hit.local_position;
		event.enter = hit.enter;
		event.leave = hit.leave;

		component_notify(hit.node->target, event);

		if (!event.is_pass_through()) {
			if (hit.leave) { // Left component has no authority to absorb the event
				AccessEventMouseMovement::reset_pass_through(event);
			} else {
				break;
			}
		}
	}

	i++;

	// Leave every other hit that was previously entered in the hit list
	for (; i < hits.size(); i++) {
		const auto& hit = hits[i];
		hit.node->inside = false;

		if (!hit.enter) { // If it's not a new enter for the component, leave that component
			event.local_position = hit.local_position;
			event.enter = false;
			event.leave = true;

			component_notify(hit.node->target, event);
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
	event.mouse_position = self->mouse_position;
	event.mouse_movement = {0, 0};
	event.enter = false;
	event.leave = true;

	// Handle acquired mouse
	if (!self->acquired.empty()) {
		event.enter = false;
		event.leave = false;
		for (const auto& entry : self->acquired) {
			event.local_position = self->mouse_position - entry->global_offset() - entry->cornerBL;
			component_notify(entry->target, event);
		}

		// No short-circuit: Continue to leave everything else too
	}

	// Gather hits
	HitELs hits;

	self->container.foreach_tree([this, &hits](MouseRegionContainer::Node& node, libv::vec2f offset) {
		const auto test_position = self->mouse_position - offset;
		const auto local_position = test_position - node.cornerBL;

		if (!node.over)
			return false;

		node.over = false;

		hits.emplace_back(&node, local_position, false, true);
		return true;
	});

	sort_hits(hits);
	notify_hits(hits, event);
}

void ContextMouse::event_button(libv::input::MouseButton mouse, libv::input::Action action) {
	// Define event
	EventMouseButton event;
	event.mouse_position = self->mouse_position;
	event.action = action;
	event.button = mouse;

	// Handle acquired mouse
	if (!self->acquired.empty()) {
		for (const auto& entry : self->acquired) {
			event.local_position = self->mouse_position - entry->global_offset() - entry->cornerBL;
			component_notify(entry->target, event);
		}
		return;
	}

	// Gather hits
	Hits hits;

	self->container.foreach_tree([this, &hits](MouseRegionContainer::Node& node, libv::vec2f offset) {
		const auto test_position = self->mouse_position - offset;
		const auto local_position = test_position - node.cornerBL;

		if (!node.over)
			return false;

		hits.emplace_back(&node, local_position);
		return true;
	});

	sort_hits(hits);
	notify_hits(hits, event);
}

namespace {

void gather_over_for_leave(HitELs& hits, MouseRegionContainer::Node& node, libv::vec2f mouse_position, libv::vec2f offset) {
	const auto node_offset = node.region_offset + offset;

	for (auto* child : node.region_nodes) {
		const auto local_position = mouse_position - node_offset - child->cornerBL;

		if (!child->over)
			continue;

		child->over = false;
		hits.emplace_back(child, local_position, false, true);

		gather_over_for_leave(hits, *child, mouse_position, node_offset);
	}
}

} // namespace

void ContextMouse::event_position(libv::vec2f position_new) {
	// Define event
	const auto movement = position_new - self->mouse_position;
	self->mouse_position = position_new;

	EventMouseMovement event;
	event.mouse_position = position_new;
	event.mouse_movement = movement;

	// Handle acquired mouse
	if (!self->acquired.empty()) {
		event.enter = false;
		event.leave = false;
		for (const auto& entry : self->acquired) {
			event.local_position = self->mouse_position - entry->global_offset() - entry->cornerBL;
			component_notify(entry->target, event);
		}
		return;
	}

	// Gather hits
	HitELs hits;

	self->container.foreach_tree([this, &hits](MouseRegionContainer::Node& node, libv::vec2f offset) {
		const auto test_position = self->mouse_position - offset;
		const auto local_position = test_position - node.cornerBL;
		const bool over_new = libv::vec::within(test_position, node.cornerBL, node.cornerTR);
		const bool over_old = node.over;

		node.over = over_new;

		if (!over_old && !over_new)
			return false;

		const bool enter = !over_old && over_new;
		const bool leave = over_old && !over_new;

		hits.emplace_back(&node, local_position, enter, leave);

		if (leave) {
			// Shortcut tree iteration as this region is left, gather every child that was 'over' to leave
			gather_over_for_leave(hits, node, self->mouse_position, offset);
			return false;
		} else {
			return true;
		}
	});

	sort_hits(hits);
	notify_hits(hits, event);
}

void ContextMouse::event_scroll(libv::vec2f movement) {
	// Define event
	self->scroll_position += movement;
	const auto position = self->scroll_position;

	EventMouseScroll event;
	event.mouse_position = self->mouse_position;
	event.scroll_position = position;
	event.scroll_movement = movement;

	// Handle acquired mouse
	if (!self->acquired.empty()) {
		for (const auto& entry : self->acquired) {
			event.local_position = self->mouse_position - entry->global_offset() - entry->cornerBL;
			component_notify(entry->target, event);
		}
		return;
	}

	// Gather hits
	Hits hits;

	self->container.foreach_tree([this, &hits](MouseRegionContainer::Node& node, libv::vec2f offset) {
		const auto test_position = self->mouse_position - offset;
		const auto local_position = test_position - node.cornerBL;

		if (!node.over)
			return false;

		hits.emplace_back(&node, local_position);
		return true;
	});

	sort_hits(hits);
	notify_hits(hits, event);
}

void ContextMouse::event_update() {
	EventMouseMovement event;
	event.mouse_position = self->mouse_position;
	event.mouse_movement = {0, 0};

	// Handle acquired mouse
	if (!self->acquired.empty())
		return;

	// Gather hits
	HitELs hits;

	self->container.foreach_tree([this, &hits](MouseRegionContainer::Node& node, libv::vec2f offset) {
		if (!node.pendingUpdate && !node.over)
			return true;

		const auto test_position = self->mouse_position - offset;
		const auto local_position = test_position - node.cornerBL;
		const bool over_new = libv::vec::within(test_position, node.cornerBL, node.cornerTR);
		const bool over_old = node.over;

		node.pendingUpdate = false;
		node.over = over_new;

		if (over_old == over_new)
			return true;

		const bool enter = !over_old && over_new;
		const bool leave = over_old && !over_new;

		hits.emplace_back(&node, local_position, enter, leave);
		return true;
	});

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
//class ImplMouseRegionHandler {
//	Interest_t interest;
//	libv::observer_ref<MouseRegion> region;
////	MouseRegionDimension dimension;
//};
//
////struct ImplContextMouse {
//class ImplMouseDimension {
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
