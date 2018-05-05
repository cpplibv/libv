// File: main.cpp, Created on 2014. április 25. 21:23, Author: Vader

// libv
#include <libv/log/log.hpp>
#include <libv/sml.hpp>
// std
#include <cassert>
#include <iostream>


// -------------------------------------------------------------------------------------------------

struct my_logger {
	template <class SM, class TEvent>
	void log_process_event(const TEvent&) {
		fmt::print("[{}][process_event] {}\n", typeid (SM).name(), typeid (TEvent).name());
	}

	template <class SM, class TGuard, class TEvent>
	void log_guard(const TGuard&, const TEvent&, bool result) {
		fmt::print("[{}][guard] {} {} {}\n", typeid (SM).name(), typeid (TGuard).name(), typeid (TEvent).name(),
				(result ? "[OK]" : "[Reject]"));
	}

	template <class SM, class TAction, class TEvent>
	void log_action(const TAction&, const TEvent&) {
		fmt::print("[{}][action] {} {}\n", typeid (SM).name(), typeid (TAction).name(), typeid (TEvent).name());
	}

	template <class SM, class TSrcState, class TDstState>
	void log_state_change(const TSrcState& src, const TDstState& dst) {
		fmt::print("[{}][transition] {} -> {}\n", typeid (SM).name(), src.c_str(), dst.c_str());
	}
};

// Dependencies
struct Sender {
	template<class TMsg>
	constexpr void send(const TMsg& msg) {
		libv::log.info("send: {}", msg.id);
	}
};

// Events
struct EventAck { bool valid{}; };
struct EventFin { int id{}; bool valid{}; };
struct EventRelease {};
struct EventTimeout {};

// Guards
constexpr auto is_valid = [](const auto& event) { return event.valid; };

// Actions
constexpr auto send_fin = [](Sender& s) { s.send(EventFin{0}); };
constexpr auto send_ack = [](const auto& event, Sender& s) { s.send(event); };

constexpr auto tt = []() { return true; };
constexpr auto ff = []() { return false; };

// State Machine
struct tcp_release : libv::sml::state_machine {

	constexpr static auto established = state<class established>;
	constexpr static auto fin_wait_1 = state<class fin_wait_1>;
	constexpr static auto fin_wait_2 = state<class fin_wait_2>;
	constexpr static auto timed_wait = state<class timed_wait>;

	/// Transition DSL: src_state + event [ guard ] / action = dst_state
	auto operator()() const {
		return table(
			established + on_entry<_>[tt]          / []{ fmt::print("established tt\n"); },
			established + on_entry<_>[ff]          / []{ fmt::print("established ff\n"); },
			established + on_entry<_>              / []{ fmt::print("established entry\n"); },
			established + on_exit<_>               / []{ fmt::print("established exit\n"); },
		   *established + on_<EventRelease>        / send_fin = fin_wait_1,
			fin_wait_1  + on_<EventAck> [is_valid]            = fin_wait_2,
			fin_wait_2  + on_<EventFin> [is_valid] / send_ack = timed_wait,
			timed_wait  + on_<EventTimeout>                   = terminate
		);
	}
};

int main() {
	std::cout << libv::logger;

	Sender s{};
	my_logger ml{};
	boost::sml::sm<tcp_release, boost::sml::logger<my_logger>> sm{s, ml}; // pass dependencies via constructor
	libv::sml::to_plantuml(sm, std::cout);

	assert(sm.is(tcp_release::established));

	sm.process_event(EventRelease{}); // complexity O(1)
	assert(sm.is(tcp_release::fin_wait_1));

	sm.process_event(EventAck{true}); // prints 'send: 0'
	assert(sm.is(tcp_release::fin_wait_2));

	sm.process_event(EventFin{42, true}); // prints 'send: 42'
	assert(sm.is(tcp_release::timed_wait));

	sm.process_event(EventTimeout{});
	assert(sm.is(tcp_release::terminate)); // terminated
}
