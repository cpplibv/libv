// Project: libv.state, File: src/libv/state/idea_state.hpp

#pragma once

//#include <libv/utility/type_uid.hpp>
#include <array>
#include <string_view>
#include <stdexcept>
#include <algorithm>
#include <map>
#include <type_traits>
#include <optional>
#include <cassert>



// === Ideal state machine library API =============================================================
//
//  This design is mostly based on boost statechart with reduced boiler plate
//      Custom react functions which can defer, transit, discard and terminate
//      in any structure are providing the most flexible API.
//      State objects are providing clean object lifetime rules.
//
//  This API requires reflection to eliminate boilerplate, but:
//      - List of every react function overload can be listed with: using reactions = list<E0, E1, E2>;
//      - List of every sub state class can be listed with: using substates = list<S0, S1, S2>;
//      - Parent state can be set with: using parent = S0; or as a template parameter of state
//
//  Open questions:
//      - How to pass parameters to a state constructor during a transition (Boost has no easy solution but a posting an internal event and reacting to that, because in FSM events should be the only data flow)
//      - Many more...
//
// -------------------------------------------------------------------------------------------------
//
//	struct MainState : libv::sm::state {
//		using initial = AwaitingAnswer;
//
//		libv::sm::result react(const EventCommunicationError& event);
//
//		struct AwaitingAnswer : libv::sm::state  {
//			result enter();
//			result react(const EventReceiveAnswer& event);
//		};
//
//		struct StandBy : libv::sm::state {
//			result react(const EventUserSubscribe& event);
//		};
//
//		struct UpdateContinuous : libv::sm::state {
//			result enter();
//			result react(const EventReceiveSOW& event);
//			result react(const EventReceiveUpdate& event);
//			result react(const EventUserUnsubscribe& event);
//			result leave();
//		};
//	};
//
//	libv::sm::result StandBy::react(const EventUserSubscribe& event) {
//		context().can_access_main_state_member_variable;
//		root_context().can_access_root_state_member_variable;
//
//		return terminate();
//		return discard_event();
//		return forward_event();
//		return defer_event();
//		return transit<UpdateContinuous>();
//	}
//
//	void main() {
//		libv::sm::machine<MainState> state_machine;
//
//		state_machine.process_event(EventReceiveAnswer{});
//		assert(state_machine.is<MainState::UpdateContinuous>());
//		state_machine.process_event(EventUserSubscribe{});
//		assert(state_machine.is<MainState::UpdateContinuous>());
//		state_machine.process_event(EventUserUnsubscribe{});
//		assert(state_machine.terminated());
//	}
//
//  Another interesting idea would be declaration only state transition definitions
//		auto react(const Event&) -> transit<struct Selected>;
//		auto react(const Event&) -> terminate;
//		auto react(const Event&) -> discard_event;
//		auto react(const Event&) -> defer_event;
//		auto react(const Event&) -> forward_event;
//  If the result type is not "result" then the framework would not call the function, but only use its declaration.
//
// -------------------------------------------------------------------------------------------------


template <typename T>
struct Sink { };

//std::array<std::type_index, 5> keys;

template <typename State, typename... Events>
static constexpr inline auto setup() {
	std::array table{
			+[](State& state, void* event) {
				state.react(*reinterpret_cast<Events*>(event));
			}...
	};

	return table;
}



// ------------------------------------------------------------------------------------------



enum class result : int {
	defer_event,
	discard_event,
	forward_event,
	terminate,
	transit,
};


template <typename... E>
struct list {
	static constexpr std::size_t size = sizeof...(E);
};


struct state {
public:
	int context();
	int root_context();

	result defer_event() {
		return result::defer_event;
	}
	result discard_event() {
		return result::discard_event;
	}
	result forward_event() {
		return result::forward_event;
	}
	result terminate() {
		return result::terminate;
	}
	template <typename T>
	result transit() {
		return result::transit;
	}

private:
	template <typename T>
	friend class state_traits;

	using states = list<>;
	using reacts = list<>;
	using initial = void;

private:
	template <typename T>
	friend class machine;

	void enter() {
	}
	void leave() {
	}
};



template <typename State>
struct state_traits;

template <>
struct state_traits<void> {
	static constexpr std::size_t num_substate = 0;
	using initial = void;
	static constexpr bool has_initial = false;
	static constexpr std::size_t sizeof_largest_live_substate = 0;
};

template <typename State>
struct state_traits {
	static constexpr std::size_t num_substate = State::states::size;
	using initial = typename State::initial;
	static constexpr bool has_initial = !std::is_void_v<initial>;

//	static constexpr std::size_t sizeof_largest_live_substate = num_substate > 0 ? sizeof(initial) + 1024 : 256;
//	static constexpr std::size_t sizeof_largest_live_substate = num_substate > 0 ? state_traits<initial>::sizeof_largest_live_substate + 1024 : 256;
	static constexpr std::size_t sizeof_largest_live_substate = has_initial ? state_traits<initial>::sizeof_largest_live_substate + 1024 : 256;
};



struct state_handler {
	void (*leave_and_destroy_state)(void*);
//	void (*destroy_state)(void*);
};



template <typename T>
struct live_state : T {
//	int active_substate_index = -1;
	const state_handler* sub_handler = nullptr;

	alignas(8)
//	alignas(128)
	[[no_unique_address]] char sub_storage[state_traits<T>::sizeof_largest_live_substate];

	template <typename SubState>
	void create_substate() {
		::new(sub_storage) live_state<SubState>();
	}

	template <typename SubState>
	void destroy_substate() {
		reinterpret_cast<SubState*>(sub_storage)->~live_state<SubState>();
	}

	template <typename SubState>
	live_state<SubState>& substate() {
		// NOTE: void* cast to suppress alignment warning
		return *static_cast<live_state<SubState>*>(
				static_cast<void*>(sub_storage));
	}

	T& state() {
		return static_cast<T&>(*this);
	}
};



template <typename State>
struct state_handler_of {
	static constexpr state_handler value{
			+[](void* storage) {
				auto& s = *reinterpret_cast<live_state<State>*>(storage);
				if (s.sub_handler != nullptr) {
					s.sub_handler->leave_and_destroy_state(s.sub_storage);
					s.sub_handler = nullptr;
				}
				s.leave();
				s.~live_state<State>();
			}
	};
};

template <typename State>
static constexpr const state_handler* state_handler_of_v = &state_handler_of<State>::value;




template <typename MainState>
struct machine {
	std::optional<live_state<MainState>> main;

private:
	template <typename State>
	void enter_state(live_state<State>& s) {
		s.enter();
		if constexpr (state_traits<State>::has_initial) {
			using initial = typename state_traits<State>::initial;
			s.sub_handler = state_handler_of_v<initial>;
			s.template create_substate<initial>();
			enter_state(s.template substate<initial>());
		}
	}

public:
	machine() {
		main.emplace();
		enter_state(*main);
	}

	~machine() {
		if (main->sub_handler != nullptr) {
			main->sub_handler->leave_and_destroy_state(main->sub_storage);
			main->sub_handler = nullptr;
		}
		main->leave();
		main.reset();
	}

	template <typename E>
	void process_event(E&& event) {
		(void) event;

//		// Idea A
//
//		auto it = &*main;
//		while (true) {
//			auto reactor = find_reactor(*it, event);
//			auto reaction = reactor->react(event);
//
//			if (reaction == result::defer_event) {
//				// ?
//				it = reactor;
//				continue;
//			}
//			if (reaction == result::terminate) {
//				terminate();
//				break;
//			}
//			if (reaction == result::discard_event) {
//				break;
//			}
//			if (reaction == result::forward_event) {
//				// ?
//			}
//			if (reaction == result::transit) {
//				transit_state(*reactor, *target_state);
//				break;
//			}
//		}
//
//
//		// Idea B
//
//      // upon state change the current event handler is updated from a 'lookup' like table
//		current_event_handler(event);
//

	}

	template <typename S>
	bool is() {
		return true;
	}

	bool terminated() {
		return true;
	}
};

// ------------------------------------------------------------------------------------------
