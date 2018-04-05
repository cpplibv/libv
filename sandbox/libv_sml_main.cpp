// File: main.cpp, Created on 2014. április 25. 21:23, Author: Vader

// ext
#include <boost/sml.hpp>
// libv
#include <libv/log/log.hpp>
// std
#include <cassert>
#include <iostream>


// Dependencies
struct Sender {
	template<class TMsg>
	constexpr void send(const TMsg& msg) {
		LIBV_LOG_LIBV_INFO("send: {}", msg.id);
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

// State Machine
struct tcp_release {
	/// Transition DSL: src_state + event [ guard ] / action = dst_state
	auto operator()() const {
		using namespace boost::sml;
		return make_transition_table(
		  *"established"_s + event<EventRelease>          / send_fin  = "fin wait 1"_s,
		   "fin wait 1"_s  + event<EventAck> [ is_valid ]             = "fin wait 2"_s,
		   "fin wait 2"_s  + event<EventFin> [ is_valid ] / send_ack  = "timed wait"_s,
		   "timed wait"_s  + event<EventTimeout>                      = X
		);
	}
};

int main() {
	std::cout << libv::log;

	Sender s{};
	boost::sml::sm<tcp_release> sm{s}; // pass dependencies via ctor
	assert(sm.is("established"_s));

	sm.process_event(EventRelease{}); // complexity O(1)
	assert(sm.is("fin wait 1"_s));

	sm.process_event(EventAck{true}); // prints 'send: 0'
	assert(sm.is("fin wait 2"_s));

	sm.process_event(EventFin{42, true}); // prints 'send: 42'
	assert(sm.is("timed wait"_s));

	sm.process_event(EventTimeout{});
	assert(sm.is(boost::sml::X)); // terminated
}
